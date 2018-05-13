#include "set_functions.h"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int h = 0;
int x;
int y;
int z;
int l;


typedef struct {
	float x;
	float y;
	float z;
} vertex;

float distance3d(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dist;
	dist = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));

	return dist;
}


//fundamental set function, not very useful on its own. Feeds a value into the array.  
void set(unsigned char row, unsigned char channel, char value)
{
	display_array[row][NUM_PLANE * 3 - 1 - channel] = value;
}//set


void delay_ms(int ms)
{

	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}





//set one whole row, note setting r, g and b values gives a shade of "white"
static void set_row(unsigned char row, char value)
{
	int rowlen = (NUM_PLANE * 3);
	int countleds;
	for (countleds = 0; countleds < rowlen; countleds++)
	{
		set(row, countleds, value);
	}

}//set_row


 //set the whole cube a single value.
static void set_all(char value)
{
	int row;
	for (row = 0; row < CUBE_SIZE; row++)
		set_row(row, value);
}//set_all

void fill(int value)
{
	set_all(value);
}

static void set_rowRGB(unsigned char row, char R, char G, char B)
{

	int ch;

	for (ch = 0; ch<64; ch++)
	{
		display_array[row][ch] = R;
	}
	for (ch = 64; ch<128; ch++)
	{
		display_array[row][ch] = G;
	}
	for (ch = 128; ch<192; ch++)
	{
		display_array[row][ch] = B;
	}

}//set_rowRGB





static void set_allRGB(char R, char G, char B)
{

	unsigned char row;
	for (row = 0; row < CUBE_SIZE; row++)
	{
		set_rowRGB(row, R, G, B);
	}

}//set_allRGB


static void set_xr(char x, char y, char z, char R, char G, char B)
{


	if (x<0)
		x = 0;
	if (x>(CUBE_SIZE - 1))
		x = CUBE_SIZE - 1;
	if (y<0)
		y = 0;
	if (y>(CUBE_SIZE - 1))
		y = CUBE_SIZE - 1;
	if (z<0)
		z = 0;
	if (z>(CUBE_SIZE - 1))
		z = CUBE_SIZE - 1;




	display_array[x][NUM_PLANE * 3 - 1 - y - z * CUBE_SIZE] = R;
	display_array[x][NUM_PLANE * 3 - 1 - NUM_PLANE - y - z * CUBE_SIZE] = G;
	display_array[x][NUM_PLANE * 3 - 1 - NUM_PLANE * 2 - y - z * CUBE_SIZE] = B;

}//set_xyzrgb


static void set_xhue(char x, char y, char z, int hue) {
	char re, gr, bl;
	hue = hue % 360;
	if (hue <= 120) {
		re = ((120 - hue) * 100) / 100;
		gr = (hue * 100) / 120;
		bl = 0;

	}
	else if (hue <= 240) {
		re = 0;
		gr = ((240 - hue) * 100) / 120;
		bl = ((hue - 120) * 100) / 120;
	}
	else {
		re = ((hue - 240) * 100) / 120;
		gr = 0;
		bl = ((360 - hue) * 100) / 120;
	}
	set_xr(x, y, z, re, gr, bl);

}//set_xhue
int icount = 0;
void setvoxel(int x, int y, int z)
{

	if (h > 360)
	{
		h = 0;
	}

	set_xhue(x, y, z, h);

	icount++;
	if (icount == 100) {
		icount = 0;
		h++;
	}




}

void setvoxelrand(int x, int y, int z)
{

	h = rand() % 360;
	set_xhue(x, y, z, h);






}
static void set_rowhue(char row, int hue)
{

	int y;
	int z;
	for (y = 0; y<8; y++)
		for (z = 0; z<8; z++)
		{
			//display_array[row][countleds] = value;
			set_xhue(row, y, z, hue);
		}

}//set_rowhue





static void set_allhue(int hue)
{
	int row;
	for (row = 0; row < CUBE_SIZE; row++)
		set_rowhue(row, hue);
}//set_all





 /*

 void hue_star()
 {

 using namespace std::literals;
 std::this_thread::sleep_for(1ms);

 //	set_allhue(funci++);
 //	if (funci == 360) funci = 1;
 //	auto lk = g_lock();

 int loop = 1000;

 int h;
 int x;
 int y;
 int z;
 int l;
 int add;
 auto lk = g_lock();
 for (l = 0; l < loop; l++)
 {
 x = (rand() % 800) / 100;
 y = (rand() % 800) / 100;
 z = (rand() % 800) / 100;
 h = (rand() % 3600) / 10;

 set_xhue(x, y, z, h);
 for (add = 0; add < 7; add++)
 {
 h = h + 30;

 set_xhue(x + add, y, z, h);
 set_xhue(x - add, y, z, h);
 set_xhue(x, y + add, z, h);
 set_xhue(x, y - add, z, h);
 set_xhue(x, y, z + add, h);
 set_xhue(x, y, z - add, h);
 set_xhue(x + add, y + add, z, h);
 set_xhue(x - add, y + add, z, h);
 set_xhue(x + add, y - add, z, h);
 set_xhue(x - add, y - add, z, h);
 set_xhue(x + add, y, z + add, h);
 set_xhue(x - add, y, z + add, h);
 set_xhue(x + add, y, z - add, h);
 set_xhue(x - add, y, z - add, h);
 //DelayMs(speed);
 std::this_thread::sleep_for(150ms);

 if (pause == 1) {
 std::this_thread::sleep_for(150000ms);
 }



 }


 for (add = 0; add < 7; add++)
 {
 std::this_thread::sleep_for(50ms);
 //DelayMs(speed / 2);
 set_xr(x + add, y, z, 0, 0, 0);
 set_xr(x - add, y, z, 0, 0, 0);
 set_xr(x, y + add, z, 0, 0, 0);
 set_xr(x, y - add, z, 0, 0, 0);
 set_xr(x, y, z + add, 0, 0, 0);
 set_xr(x, y, z - add, 0, 0, 0);

 set_xr(x + add, y + add, z, 0, 0, 0);
 set_xr(x - add, y + add, z, 0, 0, 0);

 set_xr(x + add, y - add, z, 0, 0, 0);
 set_xr(x - add, y - add, z, 0, 0, 0);

 set_xr(x + add, y, z + add, 0, 0, 0);
 set_xr(x - add, y, z + add, 0, 0, 0);

 set_xr(x + add, y, z - add, 0, 0, 0);
 set_xr(x - add, y, z - add, 0, 0, 0);



 std::this_thread::sleep_for(50ms);
 if (pause == 1) {
 std::this_thread::sleep_for(150000ms);
 }

 }
 //set_all(0);
 }
 }



 */


vertex point_rotate_around_point(vertex point, vertex center, float rotation_x, float rotation_y, float rotation_z)
{
	float x, y, z;
	float sx, cx, sy, cy, sz, cz;
	float xy, xz, yx, yz, zx, zy;
	vertex newpoint;

	// Center all the points around 0,0,0
	x = point.x - center.x;
	y = point.y - center.y;
	z = point.z - center.z;

	// Precalculate sinus and cosinus for each axis rotation
	sx = sin(rotation_x);
	cx = cos(rotation_x);

	sy = sin(rotation_y);
	cy = cos(rotation_y);

	sz = sin(rotation_z);
	cz = cos(rotation_z);


	// Rotation around x
	xy = cx * y - sx * z;
	xz = sx * y + cx * z;

	// Rotation around y
	yz = cy * xz - sy * x;
	yx = sy * xz + cy * x;

	// Rotation around z
	zx = cz * yx - sz * xy;
	zy = sz * yx + cz * xy;

	newpoint.x = zx + center.x;
	newpoint.y = zy + center.y;
	newpoint.z = yz + center.z;


	return newpoint;
}


void line_3d(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc,
		err_1, err_2, dx2, dy2, dz2;
	int pixel[3];
	pixel[0] = x1;
	pixel[1] = y1;
	pixel[2] = z1;
	dx = x2 - x1;
	dy = y2 - y1;
	dz = z2 - z1;
	x_inc = (dx < 0) ? -1 : 1;
	l = abs(dx);
	y_inc = (dy < 0) ? -1 : 1;
	m = abs(dy);
	z_inc = (dz < 0) ? -1 : 1;
	n = abs(dz);
	dx2 = l << 1;
	dy2 = m << 1;
	dz2 = n << 1;
	if ((l >= m) && (l >= n)) {
		err_1 = dy2 - l;
		err_2 = dz2 - l;
		for (i = 0; i < l; i++) {
			//PUT_PIXEL(pixel);
			setvoxel(pixel[0], pixel[1], pixel[2]);
			//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
			if (err_1 > 0) {
				pixel[1] += y_inc;
				err_1 -= dx2;
			}
			if (err_2 > 0) {
				pixel[2] += z_inc;
				err_2 -= dx2;
			}
			err_1 += dy2;
			err_2 += dz2;
			pixel[0] += x_inc;
		}
	}
	else if ((m >= l) && (m >= n)) {
		err_1 = dx2 - m;
		err_2 = dz2 - m;
		for (i = 0; i < m; i++) {
			//PUT_PIXEL(pixel);
			setvoxel(pixel[0], pixel[1], pixel[2]);
			//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
			if (err_1 > 0) {
				pixel[0] += x_inc;
				err_1 -= dy2;
			}
			if (err_2 > 0) {
				pixel[2] += z_inc;
				err_2 -= dy2;
			}
			err_1 += dx2;
			err_2 += dz2;
			pixel[1] += y_inc;
		}
	}
	else {
		err_1 = dy2 - n;
		err_2 = dx2 - n;
		for (i = 0; i < n; i++) {
			setvoxel(pixel[0], pixel[1], pixel[2]);
			//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
			//PUT_PIXEL(pixel);
			if (err_1 > 0) {
				pixel[1] += y_inc;
				err_1 -= dz2;
			}
			if (err_2 > 0) {
				pixel[0] += x_inc;
				err_2 -= dz2;
			}
			err_1 += dy2;
			err_2 += dx2;
			pixel[2] += z_inc;
		}
	}
	setvoxel(pixel[0], pixel[1], pixel[2]);
	//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
	//PUT_PIXEL(pixel);
}






void linespin(int iterations, int delay)
{
	float top_x, top_y, top_z, bot_x, bot_y, bot_z, sin_base;
	float center_x, center_y;

	center_x = 4;
	center_y = 4;

	int i, z;
	for (i = 0; i < iterations; i++)
	{

		//printf("Sin base %f \n",sin_base);

		for (z = 0; z < 8; z++)
		{

			sin_base = (float)i / 50 + (float)z / (10 + (7 * sin((float)i / 200)));

			top_x = center_x + sin(sin_base) * 5;
			top_y = center_x + cos(sin_base) * 5;
			//top_z = center_x + cos(sin_base/100)*2.5;

			bot_x = center_x + sin(sin_base + 3.14) * 10;
			bot_y = center_x + cos(sin_base + 3.14) * 10;
			//bot_z = 7-top_z;

			bot_z = z;
			top_z = z;

			//setvoxel((int) top_x, (int) top_y, 7);
			//setvoxel((int) bot_x, (int) bot_y, 0);

			//printf("P1: %i %i %i P2: %i %i %i \n", (int) top_x, (int) top_y, 7, (int) bot_x, (int) bot_y, 0);

			//line_3d((int) top_x, (int) top_y, (int) top_z, (int) bot_x, (int) bot_y, (int) bot_z);
			line_3d((int)top_z, (int)top_x, (int)top_y, (int)bot_z, (int)bot_x, (int)bot_y);
		}

		std::this_thread::sleep_for(50ms);
		set_all(0);
	}


}




void spheremove(int iterations, int delay)
{

	set_all(0);

	float origin_x, origin_y, origin_z, distance, diameter;

	origin_x = 0;
	origin_y = 3.5;
	origin_z = 3.5;

	diameter = 3;

	int x, y, z, i;

	for (i = 0; i<iterations; i++)
	{
		origin_x = 3.5 + sin((float)i / 50)*2.5;
		origin_y = 3.5 + cos((float)i / 50)*2.5;
		origin_z = 3.5 + cos((float)i / 30) * 2;

		diameter = 2 + sin((float)i / 150);

		for (x = 0; x<8; x++)
		{
			for (y = 0; y<8; y++)
			{
				for (z = 0; z<8; z++)
				{
					distance = distance3d(x, y, z, origin_x, origin_y, origin_z);
					//printf("Distance: %f \n", distance);

					if (distance>diameter && distance<diameter + 1)
					{
						setvoxel(x, y, z);
					}
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		set_all(0);
	}

}




void effect_rotate_random_pixels(int iterations, int delay, int pixels)
{


	//MSVS won't compile variable length array
	vertex points[50];
	vertex rotated[50];

	float fy, fx, fz;
	int x, y, z;
	int i, p;

	float rot_x = 0;
	float rot_y = 0;
	float rot_z = 0;
	vertex cube_center = { 3.5, 3.5, 3.5 };

	for (i = 0; i<pixels; i++)
	{
		x = rand() % 1200 - 200;
		y = rand() % 1200 - 200;
		z = rand() % 1200 - 200;
		fx = (float)x / 100;
		fy = (float)y / 100;
		fz = (float)z / 100;

		points[i].x = fx;
		points[i].y = fy;
		points[i].z = fz;

		setvoxel((int)points[i].x, (int)points[i].y, (int)points[i].z);

	}


	for (i = 0; i<iterations; i++)
	{
		rot_x = (float)i / 75;
		rot_y = (float)i / 150;
		rot_z = (float)i / 200;

		for (p = 0; p<pixels; p++)
		{
			rotated[p] = point_rotate_around_point(points[p], cube_center, rot_x, rot_y, rot_z);
		}

		fill(0x00);
		for (p = 0; p<pixels; p++)
		{
			setvoxel((int)rotated[p].x, (int)rotated[p].y, (int)rotated[p].z);
		}

		delay_ms(delay);
	}

	fill(0x00);
}