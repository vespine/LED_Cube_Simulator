//patterns.h
#ifndef patterns_H
#define patterns_H


void offline_patterns()
{
//offline patterns go here



	for (x = 0; x<CUBE_SIZE; x++)
		for (y = 0; y<CUBE_SIZE; y++)
			for (z = 0; z<CUBE_SIZE; z++)
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