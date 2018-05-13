//patterns.h
#ifndef patterns_H
#define patterns_H


void offline_patterns()
{
//offline patterns go here


	effect_rotate_random_pixels(4000, 5, 50);
	

	for (int x = 0; x < 8; x++)
	{
		line_3d(0, x, 0, 0, x, 7);
		line_3d(0, x, 7, 7, x, 7);
		line_3d(7, x, 0, 7, x, 7);
		line_3d(0, x, 0, 7, x, 0);
		
		std::this_thread::sleep_for(1000ms);
		set_all(0);
	}

	
	spheremove(1999, 10);

	linespin(2000, 1);

	for (int x = 0; x<CUBE_SIZE; x++)
		for (int y = 0; y<CUBE_SIZE; y++)
			for (int z = 0; z<CUBE_SIZE; z++)
			{
				set_xhue(x, y, z, h++);
				std::this_thread::sleep_for(10ms);
				if (x == CUBE_SIZE) x = 0;
				if (y == CUBE_SIZE) y = 0;
				if (z == CUBE_SIZE) z = 0;
				if (h == 360) h = 0;
			}



}

#endif