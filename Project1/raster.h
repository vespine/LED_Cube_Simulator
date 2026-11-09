//raster.h
//Antialiased drawing. Light is placed at exact decimal positions in a continuous
//space, and each LED shows how much of that light reaches it.
//
//Coordinates are the same as set_xr, measured in LEDs: (3.0, 0, 0) is exactly on an
//LED, (3.5, 0, 0) is halfway between two. Colours are 0.0f to 1.0f.
//
//The result ends up in display_array in the normal format, so the cube and the
//simulator show the same thing. Only needs set_xr, delay_ms, CUBE_SIZE and FRAME_MS.
#ifndef raster_H
#define raster_H

#include <math.h>
#include <string.h>
#include <atomic>

float canvas[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE][3];

void canvas_clear()
{
	memset(canvas, 0, sizeof(canvas));
}

//add light to one LED, light that falls outside the cube is simply not seen
void canvas_add(int x, int y, int z, float r, float g, float b)
{
	if (x<0 || x>(CUBE_SIZE - 1) || y<0 || y>(CUBE_SIZE - 1) || z<0 || z>(CUBE_SIZE - 1))
		return;

	canvas[x][y][z][0] += r;
	canvas[x][y][z][1] += g;
	canvas[x][y][z][2] += b;
}

//convert a canvas value to the 0-100 the cube uses. Must never go above 100: values
//from 0xF0 up are reserved as the layer markers on the Pi to PIC32 serial link.
int canvas_to_percent(float value)
{
	int percent = (int)(value * 100.0f + 0.5f);
	if (percent < 0) return 0;
	if (percent > 100) return 100;
	return percent;
}

//copy the canvas into display_array without showing it
void canvas_write()
{
	for (int x = 0; x < CUBE_SIZE; x++)
		for (int y = 0; y < CUBE_SIZE; y++)
			for (int z = 0; z < CUBE_SIZE; z++)
				set_xr(x, y, z,
					canvas_to_percent(canvas[x][y][z][0]),
					canvas_to_percent(canvas[x][y][z][1]),
					canvas_to_percent(canvas[x][y][z][2]));
}

//show the canvas as one frame, then wait for the next one
void canvas_show()
{
	canvas_write();
	delay_ms(FRAME_MS);
}

std::atomic<int> light_fade_percent{ 60 };

//how a point at fraction f of the way between two LEDs is shared between them.
//The two always add up to 1, so a point keeps its brightness as it moves.
//At 100 the share is proportional. Lower holds the first LED at full for longer
//and hands over later: at 60 the handover happens between 40% and 60% of the trip.
void axis_share(float f, float fade_distance, float& first, float& second)
{
	first = 1.0f - f / fade_distance;
	second = 1.0f - (1.0f - f) / fade_distance;
	if (first < 0.0f) first = 0.0f;
	if (second < 0.0f) second = 0.0f;

	const float total = first + second;
	if (total > 0.0f)
	{
		first /= total;
		second /= total;
	}
	else
	{
		first = 0.5f; //exactly halfway with fade at 50
		second = 0.5f;
	}
}

//a single point of light at an exact position, shared between the (up to) 8 LEDs
//around it according to how close it is to each
void draw_point(float x, float y, float z, float r, float g, float b)
{
	const float fade_distance = light_fade_percent / 100.0f;

	const int x0 = (int)floor(x);
	const int y0 = (int)floor(y);
	const int z0 = (int)floor(z);

	float share_x[2], share_y[2], share_z[2];
	axis_share(x - (float)x0, fade_distance, share_x[0], share_x[1]);
	axis_share(y - (float)y0, fade_distance, share_y[0], share_y[1]);
	axis_share(z - (float)z0, fade_distance, share_z[0], share_z[1]);

	for (int ix = 0; ix < 2; ix++)
		for (int iy = 0; iy < 2; iy++)
			for (int iz = 0; iz < 2; iz++)
			{
				const float weight = share_x[ix] * share_y[iy] * share_z[iz];
				canvas_add(x0 + ix, y0 + iy, z0 + iz, r * weight, g * weight, b * weight);
			}
}

//how far from a line the light reaches, as a percentage of the spacing between LEDs.
//At 80 the next LED stays dark for the first 20% of the trip towards it. 100 is the
//fullest blend, lower is crisper. The simulator changes this live with the arrow keys.

//a line of light. Each LED is lit by how close it is to the line: full brightness
//on the line, fading to nothing at light_fade_percent
void draw_line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
	const float fade_distance = light_fade_percent / 100.0f;
	const float dx = x2 - x1;
	const float dy = y2 - y1;
	const float dz = z2 - z1;
	const float length_squared = dx * dx + dy * dy + dz * dz;

	for (int x = 0; x < CUBE_SIZE; x++)
		for (int y = 0; y < CUBE_SIZE; y++)
			for (int z = 0; z < CUBE_SIZE; z++)
			{
				//t is how far along the line the nearest point to this LED is, 0 to 1
				float t = 0.0f;
				if (length_squared > 0.0f)
					t = (((float)x - x1) * dx + ((float)y - y1) * dy + ((float)z - z1) * dz) / length_squared;
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;

				const float distance = distance3d((float)x, (float)y, (float)z, x1 + t * dx, y1 + t * dy, z1 + t * dz);
				if (distance < fade_distance)
				{
					const float brightness = 1.0f - distance / fade_distance;
					canvas_add(x, y, z, r * brightness, g * brightness, b * brightness);
				}
			}
}

//test: a white line pinned at one corner of the front face, its other end sweeping a
//quarter turn and back like a windscreen wiper. The line is longer than the cube so
//that it reaches the far corner when it passes through the diagonal.
void raster_wiper(float seconds_per_sweep_and_back)
{
	const int frames = (int)(seconds_per_sweep_and_back * 1000.0f / FRAME_MS);
	const float length = (CUBE_SIZE - 1) * 1.5f;

	for (int frame = 0; frame < frames; frame++)
	{
		const float phase = 6.2831853f * (float)frame / (float)frames;
		const float angle = 0.78539816f * (1.0f - cosf(phase)); //0 to 90 degrees and back

		canvas_clear();
		draw_line(0.0f, 0.0f, 0.0f, length * cosf(angle), length * sinf(angle), 0.0f, 1.0f, 1.0f, 1.0f);
		canvas_show();
	}
}

//test: one white point of light tracing a circle on the front face of the cube. The
//diameter is the width of the cube, so it touches each edge and cuts inside the corners.
void raster_circle(float seconds_per_lap)
{
	const int frames = (int)(seconds_per_lap * 1000.0f / FRAME_MS);
	const float middle = (CUBE_SIZE - 1) / 2.0f;

	for (int frame = 0; frame < frames; frame++)
	{
		const float angle = 6.2831853f * (float)frame / (float)frames;

		canvas_clear();
		draw_point(middle + middle * cosf(angle), middle + middle * sinf(angle), 0.0f, 1.0f, 1.0f, 1.0f);
		canvas_show();
	}
}

//test: identical white points on every layer moving in step. With GAMMA_COMPARE on,
//each layer is displayed with a different brightness correction, to compare them
void raster_gamma_compare(float seconds_per_bounce)
{
	const int frames = (int)(seconds_per_bounce * 1000.0f / FRAME_MS);
	const float middle = (CUBE_SIZE - 1) / 2.0f;

	for (int frame = 0; frame < frames; frame++)
	{
		const float phase = 6.2831853f * (float)frame / (float)frames;

		canvas_clear();
		for (int layer = 0; layer < CUBE_SIZE; layer++)
			draw_point((float)layer, middle + middle * sinf(phase), 3.0f, 1.0f, 1.0f, 1.0f);
		canvas_show();
	}
}

//test: four points of light, each bouncing along its own row at a different speed
void raster_bouncing_points()
{
	struct Bouncer { float layer; int bounces; float r, g, b; };

	//all four line up again after LOOP_SECONDS, so "bounces" is per loop:
	//with a 24 second loop, 8 = 3s per bounce, 4 = 6s, 2 = 12s, 1 = 24s
	const float LOOP_SECONDS = 24.0f;
	const Bouncer bouncers[] = {
		{ 6.0f, 8, 1.0f, 1.0f, 1.0f }, //white, fastest
		{ 4.0f, 4, 1.0f, 0.0f, 0.0f }, //red
		{ 3.0f, 2, 0.0f, 1.0f, 0.0f }, //green
		{ 1.0f, 1, 0.0f, 0.3f, 1.0f }, //blue, slowest
	};

	const int frames = (int)(LOOP_SECONDS * 1000.0f / FRAME_MS);
	const float middle = (CUBE_SIZE - 1) / 2.0f;

	for (int frame = 0; frame < frames; frame++)
	{
		canvas_clear();

		for (const Bouncer& b : bouncers)
		{
			const float phase = 6.2831853f * (float)b.bounces * (float)frame / (float)frames;
			draw_point(b.layer, middle + middle * sinf(phase), 3.0f, b.r, b.g, b.b);
		}

		canvas_show();
	}
}

#endif
