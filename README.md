# LED_Cube_Simulator
Software simulator of an 8x8x8 RGB LED Cube in C++ and OpenGL.
The physical version can be found at http://www.vespine.com/

This code is working now, but there are several libraries and includes that have to be configured. 

This code is built entirely ontop of the fantastic examples and tutorials at https://learnopengl.com/

The goal of this software is to eventually accept input from any external pattern "source". 

My goal is to create a pattern generator on a Raspberry Pi to use for my physical cube,
instead of testing it on my cube I thought it would be a good idea to create a simulator first for testing and learning purposes. 

Work so far and goals:

Runs multithreaded, main thread is left to mostly work on render. I thought putting render in a second thread would be a good idea but I believe the OpenGL libraries are bound to the main thread, so the pattern generation is currently on a second thread. That shouls eventually turn into just data input.

Implemented transparency and anti aliasing.

investigate possibility of using opengl lighting and shading, seems unlikely at this point.

add serial interface, possibly add network itnerface. 
