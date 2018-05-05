
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include "set_functions.h"
#include "patterns.h"

#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>




//make up a lock for output stream of second thread

auto g_lock()
{
	static std::mutex m; // a global living mutex
	return std::unique_lock<decltype(m)>(m); // RAII based lock
}

//Some GLFW inits
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// display window resolution
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


float cube_transparency = 0.5f; //transparency of "off" LEDs
float scale = 0.10; // size of LED "cubes"


					//initialize some variables
float redvalue = 0;
float greenvalue = 0;
float bluevalue = 0;
float alphax = cube_transparency;
int rowi = 0;
int boxindex = 0;
int funci = 0;
bool pause = 0; //used for spacebar to pause pattern, needs work
bool threadrunning = 0; //flag for second thread
bool termin = 0; //flag for thread termination, needs work 
glm::vec3 translation;



// camera setup
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



// set up vertex data for cube object (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------


float vertxone = 1.0f; // vertex setup do not modify
float vertxtwo = 0.5f; // vertex setup do not modify

float vertone = vertxone * scale;
float verttwo = vertxtwo * scale;

float vertices[] = {
	-verttwo, -verttwo, -verttwo,  0.0f, 0.0f,
	verttwo, -verttwo, -verttwo,  vertone, 0.0f,
	verttwo,  verttwo, -verttwo,  vertone, vertone,
	verttwo,  verttwo, -verttwo,  vertone, vertone,
	-verttwo,  verttwo, -verttwo,  0.0f, vertone,
	-verttwo, -verttwo, -verttwo,  0.0f, 0.0f,

	-verttwo, -verttwo,  verttwo,  0.0f, 0.0f,
	verttwo, -verttwo,  verttwo,  vertone, 0.0f,
	verttwo,  verttwo,  verttwo,  vertone, vertone,
	verttwo,  verttwo,  verttwo,  vertone, vertone,
	-verttwo,  verttwo,  verttwo,  0.0f, vertone,
	-verttwo, -verttwo,  verttwo,  0.0f, 0.0f,

	-verttwo,  verttwo,  verttwo,  vertone, 0.0f,
	-verttwo,  verttwo, -verttwo,  vertone, vertone,
	-verttwo, -verttwo, -verttwo,  0.0f, vertone,
	-verttwo, -verttwo, -verttwo,  0.0f, vertone,
	-verttwo, -verttwo,  verttwo,  0.0f, 0.0f,
	-verttwo,  verttwo,  verttwo,  vertone, 0.0f,

	verttwo,  verttwo,  verttwo,  vertone, 0.0f,
	verttwo,  verttwo, -verttwo,  vertone, vertone,
	verttwo, -verttwo, -verttwo,  0.0f, vertone,
	verttwo, -verttwo, -verttwo,  0.0f, vertone,
	verttwo, -verttwo,  verttwo,  0.0f, 0.0f,
	verttwo,  verttwo,  verttwo,  vertone, 0.0f,

	-verttwo, -verttwo, -verttwo,  0.0f, vertone,
	verttwo, -verttwo, -verttwo,  vertone, vertone,
	verttwo, -verttwo,  verttwo,  vertone, 0.0f,
	verttwo, -verttwo,  verttwo,  vertone, 0.0f,
	-verttwo, -verttwo,  verttwo,  0.0f, 0.0f,
	-verttwo, -verttwo, -verttwo,  0.0f, vertone,

	-verttwo,  verttwo, -verttwo,  0.0f, vertone,
	verttwo,  verttwo, -verttwo,  vertone, vertone,
	verttwo,  verttwo,  verttwo,  vertone, 0.0f,
	verttwo,  verttwo,  verttwo,  vertone, 0.0f,
	-verttwo,  verttwo,  verttwo,  0.0f, 0.0f,
	-verttwo,  verttwo, -verttwo,  0.0f, vertone

};



glm::vec3 translations[512]; //array holding cube index for "translated" cubes. Used to address each individual cube once drawn 512 times. 
unsigned int VBO, VAO;





void func_2()//test thread, not implemented yet
{
	while (termin == 0)
		//test code not implemented
	{/*
	 float finv = 0.0f;
	 float stv = 0.0f;
	 float aa = 0;
	 float bb = 0;
	 float cc = 0;
	 float dd = 0;
	 float ee = 0;
	 float ff = 0;
	 float gg = 0;
	 float hh = 0;
	 float aa2 = 0;
	 float bb2 = 0;
	 float cc2 = 0;
	 float dd2 = 0;
	 float ee2 = 0;
	 float ff2 = 0;
	 float gg2 = 0;
	 float hh2 = 0;



	 for (int i = 0; i < 8; i++)
	 {

	 aa = display_buffer[8 * i][63];
	 bb = display_buffer[8 * i + 1][63] * 8 / 8;
	 cc = display_buffer[8 * i + 2][63] * 7 / 8;
	 dd = display_buffer[8 * i + 3][63] * 6 / 8;
	 ee = display_buffer[8 * i + 4][63] * 5 / 8;
	 ff = display_buffer[8 * i + 5][63] * 4 / 8;
	 gg = display_buffer[8 * i + 6][63] * 3 / 8;
	 hh = display_buffer[8 * i + 7][63] * 2 / 8;
	 bb2 = display_buffer[8 * i - 1][63] * 8 / 8;
	 cc2 = display_buffer[8 * i - 2][63] * 7 / 8;
	 dd2 = display_buffer[8 * i - 3][63] * 6 / 8;
	 ee2 = display_buffer[8 * i - 4][63] * 5 / 8;
	 ff2 = display_buffer[8 * i - 5][63] * 4 / 8;
	 gg2 = display_buffer[8 * i - 6][63] * 3 / 8;
	 hh2 = display_buffer[8 * i - 7][63] * 2 / 8;
	 finv = aa + bb + cc + dd + ee + ff + gg + hh + aa2 + bb2 + cc2 + dd2 + ee2 + ff2 + gg2 + hh2;


	 display_array[i][56] = finv;

	 std::this_thread::sleep_for(1ms);

	 }
	 */

	}
}





void func_1() //pattern drawring thread. 

{


	//test_pattern
	
	/*
	while (1 == 1)
	{
	for (int l = 0; l<100; l++)
	{
	for (int x = 0; x<8; x++)
	for (int y = 0; y<8; y++)
	for (int z = 0; z<8; z++)
	{
	set_xhue(x, y, z, h++);
	if (x == 8) x = 0;
	if (y == 8) y = 0;
	if (z == 8) z = 0;

	std::this_thread::sleep_for(10ms);
	}
	}//for loop
	}
	*/










	threadrunning = TRUE; //init thread flag
	
	while (termin == 0) //this doesn't work properly, the solution is std::promise and std::future, need to figure this out 
	{

		HANDLE hComm;                          // Handle to the Serial port
		char  ComPortName[] = "\\\\.\\COM1";  // Name of the Serial port(May Change) to be opened,
		BOOL  Status;                          // Status of the various operations 
		DWORD dwEventMask;                     // Event mask to trigger
		unsigned char  TempChar;               // Temperory Character
		char  SerialBuffer[384];               // Buffer Containing Rxed Data
		DWORD NoBytesRead;                     // Bytes read by ReadFile()
		int i = 0;

		printf("\n\n +==========================================+");
		printf("\n |    Serial Port  Reception (Win32 API)    |");
		printf("\n +==========================================+\n");
		//---------------------------------- Opening the Serial Port -------------------------------------------

		hComm = CreateFile(ComPortName,                  // Name of the Port to be Opened
			GENERIC_READ | GENERIC_WRITE, // Read/Write Access
			0,                            // No Sharing, ports cant be shared
			NULL,                         // No Security
			OPEN_EXISTING,                // Open existing port only
			0,                            // Non Overlapped I/O
			NULL);                        // Null for Comm Devices

		if (hComm == INVALID_HANDLE_VALUE)
			printf("\n    Error! - Port %s can't be opened\n", ComPortName);
		else
			printf("\n    Port %s Opened\n ", ComPortName);

		//------------------------------- Setting the Parameters for the SerialPort ------------------------------

		DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		Status = GetCommState(hComm, &dcbSerialParams);      //retreives  the current settings

		if (Status == FALSE)
			printf("\n    Error! in GetCommState()");

		dcbSerialParams.BaudRate = CBR_115200;    // Setting BaudRate = CBR_115200;
		dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
		dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
		dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

		Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

		if (Status == FALSE)
		{
			printf("\n    Error! in Setting DCB Structure");
		}
		else //If Successfull display the contents of the DCB Structure
		{
			printf("\n\n    Setting DCB Structure Successfull\n");
			printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
			printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
			printf("\n       StopBits = %d", dcbSerialParams.StopBits);
			printf("\n       Parity   = %d", dcbSerialParams.Parity);
		}

		//------------------------------------ Setting Timeouts --------------------------------------------------

		COMMTIMEOUTS timeouts = { 0 };
		timeouts.ReadIntervalTimeout = 50;
		timeouts.ReadTotalTimeoutConstant = 50;
		timeouts.ReadTotalTimeoutMultiplier = 10;
		timeouts.WriteTotalTimeoutConstant = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;

		if (SetCommTimeouts(hComm, &timeouts) == FALSE)
			printf("\n\n    Error! in Setting Time Outs");
		else
			printf("\n\n    Setting Serial Port Timeouts Successfull");

		//------------------------------------ Setting Receive Mask ----------------------------------------------

		Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

		if (Status == FALSE)
			printf("\n\n    Error! in Setting CommMask");
		else
			printf("\n\n    Setting CommMask successfull");


		//------------------------------------ Setting WaitComm() Event   ----------------------------------------




		while (1 == 1)
		{

			printf("\n\n    Waiting for Data Reception");

			Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

			//-------------------------- Program will Wait here till a Character is received ------------------------
			printf("\n\n    Waiting for Data Reception");

			Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

			if (Status == FALSE)
			{
				printf("\n    Error! in Setting WaitCommEvent()");
			}
			else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
			{
				//printf("\n\n    Characters Received");




				float cvalue;
				char d;
				d = ' ';
				while (1 == 1)
				{


					Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);



					if (TempChar > 0xef)
					{
						char currentrow = TempChar - 0xf0;
						printf("\n    new row");
						printf("%d", currentrow);

						for (unsigned char cchan = 0; cchan < 192; cchan++)
						{

							Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
							printf("%x", TempChar);
							//printf("%d", currentrow);


							cvalue = TempChar;
							cvalue = cvalue / 100;

							printf("%c", d);
							printf("%f", cvalue);
							printf("%c", d);

							display_array[currentrow][cchan] = cvalue;


						}


					}


				}


			}

		}



		CloseHandle(hComm);//Closing the Serial Port
		printf("\n +==========================================+\n");
		//_getch();













	}//termin


	
}






int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LED Cube", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//glEnable(GL_DEPTH_TEST); //culling faces does not work as expected for translucent cubes, looks better with it off.
	glEnable(GL_BLEND); //required for translucency 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("1LedCube.vs", "1LedCube.fs");


	//configure buffer objects 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	// world space positions of our cubes

	int index = 0;
	float offset = 0.1f;

	for (int y = -8; y < 8; y += 2)
	{
		for (int z = -8; z < 8; z += 2)
		{
			for (int x = -8; x < 8; x += 2)
			{
				//glm::vec3 translation;
				translation.x = (float)x / 5.0f + offset;
				translation.y = (float)y / 5.0f + offset;
				translation.z = (float)z / 5.0f + offset;
				translations[index++] = translation;
			}
		}
	}

	//start parallel threads
	std::thread t1(func_1);
	//std::thread t2(func_2);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);




		//sort needs to go here

		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < 512; i++)
		{
			float distance = glm::length(camera.Position - translation[i]);
			sorted[distance] = translations[i];
		}







		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// activate shader
		ourShader.use();



		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);




		// render boxes
		glBindVertexArray(VAO);

		/*sorting not working
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
		glm::mat4 model;
		model = glm::translate(model, it->second);
		ourShader.setMat4("model", model);
		rowi = boxindex / 64;
		//using mod operation to get individual R G and B values from the display_array to the correct "box index"
		redvalue = display_array[rowi][boxindex % 64];
		greenvalue = display_array[rowi][boxindex % 64 + 64];
		bluevalue = display_array[rowi][boxindex % 64 + 128];
		//this passes our R G and B values to the vertex shader
		ourShader.setFloat("red", redvalue);
		ourShader.setFloat("green", greenvalue);
		ourShader.setFloat("blue", bluevalue);
		//I wanted the cubes to be more transparent when off but not so transparent when on.
		//just noticed transparency works from one direction but is wrong when looking from the other side.
		alphax = cube_transparency + (redvalue + bluevalue + greenvalue) / 3;
		//if (alphax > 1.0) { alphax = 1.0f; }
		ourShader.setFloat("alphaf", alphax);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		*/

		for (boxindex = 0; boxindex < 512; boxindex++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, translations[boxindex]);
			float angle = 1.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			//box index is just one long string of 512 LED 'indexes', each layer or "row" is 64 indexes long:
			rowi = boxindex / 64;

			//using mod operation to get individual R G and B values from the display_array to the correct "box index"
			redvalue = display_array[rowi][boxindex % 64];
			greenvalue = display_array[rowi][boxindex % 64 + 64];
			bluevalue = display_array[rowi][boxindex % 64 + 128];

			//this passes our R G and B values to the vertex shader 
			ourShader.setFloat("red", redvalue);
			ourShader.setFloat("green", greenvalue);
			ourShader.setFloat("blue", bluevalue);

			//I wanted the cubes to be more transparent when off but not so transparent when on. 
			//just noticed transparency works from one direction but is wrong when looking from the other side. 
			alphax = cube_transparency;// +((redvalue + bluevalue + greenvalue) / 6);


									   //if (alphax > 1.0) { alphax = 1.0f; }
			ourShader.setFloat("alphaf", alphax);






			glDrawArrays(GL_TRIANGLES, 0, 36);

		}



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	termin = 1;
	t1.join();
	glfwTerminate();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pause = !pause;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
