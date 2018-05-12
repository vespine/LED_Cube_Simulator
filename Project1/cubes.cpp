#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/mesh.h>

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>


#define ONLINE FALSE		//RUN MODE ONLINE: TRUE = from network, FALSE = internal patters from patterns.h
#define PATTERN_SERVER "raspberrypi" //host name of LED CUBE Pattern generator
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080
#define TRANSPARENCY 0.1f	//Blank LED "opaqueness" : 0.0f-1.0f (0% = invisible - 100% = solid), "lit" leds are proportionatelly less translucent.
#define CUBE_SIZE 8 // 8 is default size, changing this value is not completely implemented. Use values above 32 at your own risk.
#define LED_SCALE 1.0f		//default 1.0f
#define SIZE_SCALE 1.0f		//default 1.0f

const int NUM_PLANE = CUBE_SIZE * CUBE_SIZE;
const int NUM_CUBE = CUBE_SIZE * CUBE_SIZE * CUBE_SIZE;

char display_array[CUBE_SIZE][NUM_PLANE * 3];

#include "set_functions.h"
#include "patterns.h"

using boost::asio::ip::tcp;

//thread lock

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

//initialize some flags
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
float vertxone = 0.1f; // vertex setup do not modify
float vertxtwo = 0.05f; // vertex setup do not modify

float vertone = vertxone * LED_SCALE;
float verttwo = vertxtwo * LED_SCALE;

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

glm::vec3 translations[NUM_CUBE]; //array holding cube index for "translated" cubes. Used to address each individual cube once drawn 512 times. 
unsigned int VBO, VAO;

void func_2()//test thread, not implemented yet
{
	while (termin == 0)
		//test code not implemented

	{
		/*


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

void func_1() //pattern drawring / network input thread. 
{
	threadrunning = TRUE; //init thread flag

	if (ONLINE != TRUE)
	{
		while (termin == 0)
		{
			offline_patterns();
		}
	}

	if (ONLINE == TRUE)
	{
		//online run mode, connect to pattern server
		try
		{
			boost::asio::io_service io_service;

			tcp::resolver resolver(io_service);
			tcp::resolver::query query(PATTERN_SERVER, "daytime");
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			tcp::socket socket(io_service);

			//int buffcount = 0;
			while (termin == 0)
			{
				//frame count, uses int buffcount = 0 above
				//std::cout << "received buffer" << buffcount << std::endl;
				//buffcount++;

				boost::asio::connect(socket, endpoint_iterator);
				boost::system::error_code error;
				size_t len = socket.read_some(boost::asio::buffer(display_array), error);

				std::this_thread::sleep_for(2ms); //prevent free running loop in thread
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}


int main()
{
	cout << "resolution set to " << SCR_WIDTH << " x " << SCR_HEIGHT << endl;

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
	//glEnable(GL_DEPTH_TEST); //culling faces does not work as I expected for translucent cubes, looks better with it off.
	glEnable(GL_BLEND); //required for translucency 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	// build and compile shader zprogram
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
	
	// generate world space positions of our cubes

	int index = 0;
	float offset = SIZE_SCALE;

	for (int y = -CUBE_SIZE; y < CUBE_SIZE; y += 2)
	{
		for (int z = -CUBE_SIZE; z < CUBE_SIZE; z += 2)
		{
			for (int x = -CUBE_SIZE; x < CUBE_SIZE; x += 2)
			{
				//glm::vec3 translation;
				translation.x = (float)x / 4.0f*offset;
				translation.y = (float)y / 4.0f*offset;
				translation.z = (float)z / 4.0f*offset;
				translations[index++] = translation;
			}
		}
	}

	//start parallel threads
	std::thread t1(func_1);
	//std::thread t2(func_2); //not implemented

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		//sort needs to go here

		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < NUM_CUBE; i++)
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

		for (int boxindex = 0; boxindex < NUM_CUBE; boxindex++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, translations[boxindex]);
			//float angle = 1.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			//box index is just one long string of NUM_CUBE LED 'indexes', each layer or "row" is NUM_PLANE indexes long:
			int rowi = boxindex / (NUM_PLANE);

			//using mod operation to get individual R G and B values from the display_array to the correct "box index"
			float redvalue = display_array[rowi][boxindex % NUM_PLANE];
			float greenvalue = display_array[rowi][boxindex % NUM_PLANE + NUM_PLANE];
			float bluevalue = display_array[rowi][boxindex % NUM_PLANE + (NUM_PLANE*2)];

			//display_array is "ints" in physical cube, but floats in openGL, below turns 0-100, to 0.0f-1.0f
			redvalue = redvalue / 100;
			greenvalue = greenvalue / 100;
			bluevalue = bluevalue / 100;
			
			//this passes our R G and B values to the vertex shader 
			ourShader.setFloat("red", redvalue);
			ourShader.setFloat("green", greenvalue);
			ourShader.setFloat("blue", bluevalue);
						
			//just noticed transparency works from one direction but is wrong when looking from the other side. 
			
			//chose highstest value out of RGB and give the cube that alpha(opaqueness) value. 
			float alphax = redvalue;
			if (alphax < bluevalue){
				alphax = bluevalue;
			};

			if (alphax < greenvalue){
				alphax = greenvalue;
			};
				alphax = alphax*(1-TRANSPARENCY) + TRANSPARENCY;
	
			ourShader.setFloat("alphaf", alphax);
			//finally draw the cube
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
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = (float)lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
