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

#include <string>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>


//make up a lock for output stream

auto g_lock()
{
	static std::mutex m; // a global living mutex
	return std::unique_lock<decltype(m)>(m); // RAII based lock
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


float cube_transparency = 0.08f; //transparency of "off" LEDs
float scale = 0.14; // size of LED "cubes"


//initialize some variables
float redvalue = 0;
float greenvalue = 0;
float bluevalue = 0;
float alphax = cube_transparency;
int rowi = 0;
int boxindex = 0;
int funci = 0;
bool pause = 0; //used for space to pause pattern
bool threadrunning = 0; 
bool termin = 0; //thread termination 




// camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



// set up vertex data (and buffer(s)) and configure vertex attributes
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

glm::vec3 translations[512];
unsigned int VBO, VAO;






		
void func_2()//test thread
{
	while (termin == 0)
	{
		display_array[0][0] = 0.01f;
		display_array[0][0] = display_buffer[0][0];

	}
}





void func_1() //pattern drawring thread
{
	threadrunning = TRUE;
	while (termin == 0)
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
			for (add = 0; add < 6; add++)
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


			for (add = 0; add < 9; add++)
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

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




	//int ledloop = 0;
	// world space positions of our cubes
	//glm::vec3 translation;
	int index = 0;
	float offset = 0.1f;

	for (int y = -8; y < 8; y += 2)
	{
		for (int z = -8; z < 8; z += 2)
		{
			for (int x = -8; x < 8; x += 2)
			{
				glm::vec3 translation;
				translation.x = (float)x / 5.0f + offset;
				translation.y = (float)y / 5.0f + offset;
				translation.z = (float)z / 5.0f + offset;
				translations[index++] = translation;
			}
		}
	}

	std::thread t1(func_1);
	std::thread t2(func_2);

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
		for (boxindex = 0; boxindex < 512; boxindex++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, translations[boxindex]);
			float angle = 1.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

						
			rowi =  boxindex /64;
						
			redvalue = display_array[rowi][boxindex%64];
			greenvalue = display_array[rowi][boxindex%64 + 64];
			bluevalue = display_array[rowi][boxindex%64 + 128];
		

			ourShader.setFloat("red", redvalue);
			ourShader.setFloat("green", greenvalue);
			ourShader.setFloat("blue", bluevalue);

			//transparency is inversely proportional to color value.
			alphax = cube_transparency +(redvalue + bluevalue + greenvalue) / 3;
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