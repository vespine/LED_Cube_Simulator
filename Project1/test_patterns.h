//test_patterns.h
//Saved test patterns for judging how the antialiased drawing in raster.h looks.
//To run them, set RUN_RASTER_TEST to 1 in cubes.cpp (0 goes back to patterns.h).
//
//Simulator keys: 1-5 choose a test, Up/Down fade, Left/Right speed, Space pause.
//
//Findings so far, on screen (to be repeated on the real cube):
//  - A fade of 65% looks best for lines, spheres, sheets and balls. Less glow reads
//    sharper than more.
//  - A lone point looks steppy at every setting, even 90%. Avoid points unless that is
//    the look you want, and use a small ball instead (test 5 is a good example).
#ifndef test_patterns_H
#define test_patterns_H

//The tests below take the animation time in seconds, so the speed can change live.
//All of them are white.

//a line pinned at one corner of the front face, its other end sweeping a quarter turn
//and back like a windscreen wiper. The line is longer than the cube so that it reaches
//the far corner when it passes through the diagonal.
void test_wiper(float t)
{
	const float length = (CUBE_SIZE - 1) * 1.5f;
	const float angle = 0.78539816f * (1.0f - cosf(6.2831853f * t / 10.0f)); //0 to 90 degrees and back

	draw_line(0.0f, 0.0f, 0.0f, length * cosf(angle), length * sinf(angle), 0.0f, 1.0f, 1.0f, 1.0f);
}

//one point tracing a circle on the front face of the cube. The diameter is the width
//of the cube, so it touches each edge and cuts inside the corners.
void test_circle(float t)
{
	const float middle = (CUBE_SIZE - 1) / 2.0f;
	const float angle = 6.2831853f * t / 10.0f;

	draw_point(middle + middle * cosf(angle), middle + middle * sinf(angle), 0.0f, 1.0f, 1.0f, 1.0f);
}

//a hollow ball growing from the centre out past the corners of the cube, and back
void test_sphere(float t)
{
	const float middle = (CUBE_SIZE - 1) / 2.0f;
	const float largest = middle * 1.7320508f + 0.15f; //just past the corners
	const float radius = largest * 0.5f * (1.0f - cosf(6.2831853f * t / 8.0f));

	draw_sphere(middle, middle, middle, radius, 1.0f, 1.0f, 1.0f);
}

//a flat sheet sweeping through the whole cube and back. It is deliberately tilted at
//an uneven angle so that every LED crosses it at a slightly different moment.
void test_sheet(float t)
{
	const float nx = 1.0f, ny = 0.7320508f, nz = 0.4142136f; //ratios that never line up with the LED grid
	const float far_corner = (CUBE_SIZE - 1) * (nx + ny + nz) / sqrtf(nx * nx + ny * ny + nz * nz);
	const float position = far_corner * 0.5f - (far_corner * 0.5f + 1.0f) * cosf(6.2831853f * t / 10.0f);

	draw_sheet(nx, ny, nz, position, 1.0f, 1.0f, 1.0f);
}

//a solid ball 2 LEDs across, orbiting in a circle tilted across the cube's main
//diagonal. The orbit is sized so the whole ball stays inside the cube.
void test_ball_orbit(float t)
{
	const float middle = (CUBE_SIZE - 1) / 2.0f;
	const float orbit_radius = 3.0f;
	const float angle = 6.2831853f * t / 8.0f;
	const float c = cosf(angle) * orbit_radius;
	const float s = sinf(angle) * orbit_radius;

	//two directions at right angles, both flat in the plane facing corner (7,7,7)
	const float ux = 0.70710678f, uy = -0.70710678f, uz = 0.0f;
	const float vx = 0.40824829f, vy = 0.40824829f, vz = -0.81649658f;

	draw_ball(middle + c * ux + s * vx, middle + c * uy + s * vy, middle + c * uz + s * vz, 1.0f, 1.0f, 1.0f, 1.0f);
}

const char* raster_test_names[] = { "", "Circle", "Wiper", "Sphere", "Tilted sheet", "Ball orbit" };
const int RASTER_TEST_COUNT = 5;
std::atomic<int> active_test{ 5 }; //number keys 1 to 5 in the simulator
std::atomic<int> speed_percent{ 100 }; //left and right arrows in the simulator

//draw and show one frame of whichever test is selected
void raster_test_frame()
{
	static float t = 0.0f;

	canvas_clear();
	switch (active_test)
	{
	case 1: test_circle(t); break;
	case 2: test_wiper(t); break;
	case 3: test_sphere(t); break;
	case 4: test_sheet(t); break;
	case 5: test_ball_orbit(t); break;
	}
	canvas_show();

	t += (FRAME_MS / 1000.0f) * (speed_percent / 100.0f);
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
