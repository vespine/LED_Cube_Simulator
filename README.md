# LED_Cube_Simulator
Software simulator of an 8x8x8 RGB LED Cube in C++ and OpenGL.
The physical version can be found at http://www.vespine.com/

This code is working now, but there are several libraries and includes that have to be configured. 

This code is built entirely ontop of the fantastic examples and tutorials at https://learnopengl.com/

The goal of this software is to eventually accept input from any external pattern "source". 

My goal is to create a pattern generator on a Raspberry Pi to use for my physical cube,
instead of testing it on my cube I thought it would be a good idea to create a simulator first for testing and learning purposes. 

Work so far and goals:

Runs multithreaded, main thread is left to mostly work on render, separate thread is being used to generate the patterns.

Implemented transparency

investigate possibility of using lighting, seems unlikely.

add serial interface, possibly add network itnerface. 
