//set_functions.h
#ifndef set_functions_H
#define set_functions_H



#define NUM_TLCS   12 //how many TLCs in serial, (TLC = 15 channels) this is taken from the arduino tlc library.
#define NUM_ROWS   8 //how many multiplexed rows


float display_array[NUM_ROWS][NUM_TLCS * 16];// 8 * 192 (64r-64g-64b)this is where the LED data is actually stored. Channel value is a float between 0 (0%) to 1.0f (100%).

float display_buffer[NUM_ROWS*8][NUM_TLCS * 16 * 8];

//Each row can be thought of a chess board 8x8 LEDs, each LED has 3 separate values one for each of R G and B. So each "row" has 3 chess boards of values. 


void set(unsigned char row, unsigned char channel, float value) //very simple! set a single value to a specified spot on the matrix. 
{
display_array[row][NUM_TLCS * 16 - 1 - channel] = value;
}//set




static void set_row(unsigned char row, float value)
{
	int rowlen = (NUM_TLCS * 16);
	int countleds;
	for (countleds = 0; countleds < rowlen; countleds++)
	{
		set(row, countleds, value);
	}

}//set_row



static void set_all(float value)
{
	int row;
	for (row = 0; row < NUM_ROWS; row++)
		set_row(row, value);
}//set_all


static void set_rowRGB(unsigned char row, float R, float G, float B)
{

	int ch;

	for (ch = 0; ch<64; ch++)
	{
		display_array[row][ch] = B;
	}
	for (ch = 64; ch<128; ch++)
	{
		display_array[row][ch] = G;
	}
	for (ch = 128; ch<192; ch++)
	{
		display_array[row][ch] = R;
	}

}//set_rowRGB





static void set_allRGB(float R, float G, float B)
{

	unsigned char row;
	for (row = 0; row < NUM_ROWS; row++)
	{
		set_rowRGB(row, R, G, B);
	}

}//set_allRGB


static void set_xr(char x, char y, char z, float R, float G, float B)
{


	if (x<0)
		x = 0;
	if (x>7)
		x = 7;
	if (y<0)
		y = 0;
	if (y>7)
		y = 7;
	if (z<0)
		z = 0;
	if (z>7)
		z = 7;




	display_array[x][NUM_TLCS * 16 - 1 - y - z * 8] = R;
	display_array[x][NUM_TLCS * 16 - 1 - 64 - y - z * 8] = G;
	display_array[x][NUM_TLCS * 16 - 1 - 128 - y - z * 8] = B;

}//set_xyzrgb


static void set_xhue(char x, char y, char z, int hue) {
	float re, gr, bl;
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
	set_xr(x, y, z, re / 100, gr / 100, bl / 100);

}//set_xhue




static void set_rowhue(unsigned char row, int hue)
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
	for (row = 0; row < NUM_ROWS; row++)
		set_rowhue(row, hue);
}//set_all

int stepper = 0;
int huec = 0;


float h = 0;
int x;
int y;
int z;
int l;




void hue_run(int loop, int speed)
{

	for (l = 0; l<loop; l++)
	{
		for (x = 0; x<8; x++)
			for (y = 0; y<8; y++)
				for (z = 0; z<8; z++)
				{
					set_xhue(x, y, z, h++);
					if (x == 8) x = 0;
					if (y == 8) y = 0;
					if (z == 8) z = 0;






				}

	}//for loop
}//cross_star


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






#endif