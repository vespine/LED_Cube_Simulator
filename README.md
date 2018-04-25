# LED_Cube_Simulator
Software simulator of an 8x8x8 RGB LED Cube in C++ and OpenGL.

The physical version can be found at http://www.vespine.com/ 

This code is working now, but there are several libraries and includes that have to be configured, all included in the repository. 
Short demo video is on youtube https://youtu.be/7ydev2SXP-s

This code is built entirely ontop of the fantastic examples and tutorials at https://learnopengl.com/

The goal of this software is to eventually accept input from any external pattern "source". 

I am NOT a "programmer" and I'm not very good at any programming language, so please forgive my lack of style and function. 

I want to create a LED Cube "pattern generator" on a Raspberry Pi to use for my physical cube, but instead of having to test it on the real cube, which is big and hard to move around, I thought it would be a good idea to create a simulator first, for testing but also for learning purposes. I looked around and although it appears a few people have programmed "pettern generators" for their own cubes, I could not actually find any existing LED Cube Simulators and certainly none that are open source or documented. 


Runs multithreaded, main thread is left to mostly work on render. I thought putting render in a second thread would be a good idea but I believe the OpenGL libraries are bound to the main thread. So instead it's the pattern generation which currently on a second thread. I will probably leave a few small patterns inside the softrware just for functionality testing but the goal is to use an external source for the patterns. 

Implemented transparency and anti aliasing.

Need to investigate possibility of using opengl lighting and shading, seems unlikely at this point, quite complicated and not sure how much "better" it would look, if at all. 

Add serial interface, possibly add network itnerface. This will be the main input for the patterns.

Need to clean up all the redundant and unneccesary files. I used a lot of files from learnopengl to get learn how to get to this stage but they are not required in this project. 

Data structure overview:

The cube is represented by a data array display_array[NUM_ROWS][NUM_TLCS * 16]

This is how it is represented in my phyical cube, there are 8 rows and 12 "TLCS" which are the LED Drivers I use (TLC5940). Each driver is 16 channels, making the array:

display_array[8][192]

8 represents the 8 physical horizontal "layers" of the cube, each layer can be thought of as a "chess board" of 8 by 8 (64) leds. So 192 represents the 64 red, 64 green and 64 blue "values" of the layer all in a row. There is any number of ways the cube can be represented, it could have been display_array[3][8][8][8] might have made the most sense, but the data is sent to the drivers layerwise so having the whole layer in one array made sense. 

In the physical cube, the actual array is made of "ints" but only hold 12bit values instead of the maximum 16 bits an int can hold, because the TLC LED Drivers only have 12bit resolution. In practice, this is FAR too much resolution for a LED cube, I decided to work with 0-100 values instead so my "set" functions all translate percentage to 12 bit when moving values to the array. That STILL gives a million possible colors and is more than enough, it would probably work fine with a tenth if not even a hundredth the colors. 

For the simulator, in OpenGL, the color is set by a "float" which is 0 - 1.0, you "can" use resolution better than 2 decimal places, but  percentage to float still translates very nicely.

For my array, to make the very first LED white, you would set display_array [0][0], display_array[0][64] and dislplay_array[0][128] to 1. To move the LED along the x, y or z axis, you add [x][0], [0][y] or [0][z*8] respectively.

I have created several functions to make setting single points, rows and the whole cube easy, they are in "set_functions.h"
