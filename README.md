# LED_Cube_Simulator
Software simulator of an 8x8x8 RGB LED Cube in C++ and OpenGL.

This is a LED Cube simulator that you can use for creating and testing LED Cube patterns. You can run it in "stand alone" mode, writing the patterns directly into this program, or it also accepts a network input, implemented with BOOST ASIO.

I have also created a Raspberry Pi based LED Cube pattern generator that is configured to feed this simulator. The RPi will eventually end up in my physical cube as the pattern generator. 

The physical version can be found at http://www.vespine.com/ 

Short demo video is on youtube https://youtu.be/7ydev2SXP-s

This code is built almost entirely on top of the fantastic examples and tutorials at https://learnopengl.com/

I am not really a "programmer" so please forgive my lack of style and function. 

I want to create a LED Cube "pattern generator" on a Raspberry Pi to use for my physical cube. Instead of having to test it on the real cube, which is big and hard to move around, I thought it would be a good idea to create a simulator first, for testing but also for learning purposes. I looked around and although it appears a few people have programmed "pettern generators" for their own cubes, I could not actually find any existing LED Cube Simulators and certainly none that are open source or documented. 

Data structure overview:

The cube is represented by a 2d array display_array[8][192]

8 represents the 8 physical horizontal "layers" of the cube, each layer can be thought of as a "chess board" of 8 by 8 (64) leds. So 192 represents the 64 red, 64 green and 64 blue "values" of the layer all in a row. There is any number of ways the cube can be represented, it could have been display_array[3][8][8][8] might have made the most sense, but the data is sent to the drivers layerwise so having the whole layer in one array made sense. 

In the physical cube, the actual array is made of "ints" but only hold 12bit values instead of the maximum 16 bits an int can hold, because the TLC LED Drivers only have 12bit resolution. In practice, this is FAR too much resolution for a LED cube, I decided to work with 0-100 values instead so my "set" functions all translate "percentage" to 12 bit when moving values to the array. That STILL gives a million possible colors and is more than enough, it would probably work fine with a tenth if not even a hundredth the colors. 

For the simulator, in OpenGL, the color is set by a "float" which is 0 - 1.0, you "can" use resolution better than 2 decimal places, but  percentage to float translates very nicely.

For my array, to make the very first LED white, you would set display_array [0][0], display_array[0][64] and dislplay_array[0][128] to 1.0, To move the LED along the x, y or z axis, you add [x][0], [0][y] or [0][z*8] respectively.

I have created several functions to make setting single points, rows and the whole cube easy, they are in "set_functions.h"
