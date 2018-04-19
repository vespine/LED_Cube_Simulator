#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>

#define NUM_TLCS   12 //how many TLCs in serial, this is taken from the arduino tlc library
#define NUM_ROWS   8 //how many multiplexed rows

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



float display_array[NUM_ROWS][NUM_TLCS * 16];// 8 * 192 (64r-64g-64b)this is where the LED data is actuall stored


void set(unsigned char row, unsigned char channel, float value) //very simple! 
{
	display_array[row][NUM_TLCS * 16 - 1 - channel] = value;
}//set


static void set_row(unsigned char row, float value)
{
	int rowlen = (NUM_TLCS * 16);
	int countleds;
	for (countleds = 0; countleds < rowlen; countleds++)
	{
		//display_array[row][countleds] = value;
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
	set_xr(x, y, z, re/100, gr/100, bl/100);

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





void hue_run(int loop, int speed)
{
	float h;
	int x;
	int y;
	int z;
	int l;

	h = 0;
	for (l = 0; l<loop; l++)
	{
		for (x = 0; x<8; x++)
			for (y = 0; y<8; y++)
				for (z = 0; z<8; z++)
				{
					set_xhue(x, y, z, h);
					h = h + 1;

				}

	}//for loop
}//cross_star









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
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("1LedCube.vs", "1LedCube.fs");


	//

	//	for(unsigned int i = 0; i < 100; i++)
	//{
	//    stringstream ss;
	//    string index;
	//    ss << i; 
	//    index = ss.str(); 
	//    shader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
	//}  


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//cube scale

	float scale = 0.06; // size of LED "cubes"


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

	// world space positions of our cubes
	glm::vec3 translations[512];

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































	unsigned int VBO, VAO;
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


	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);


	//int ledloop = 0;


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

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		ourShader.use();




		// update the uniform color
		//float timeValue = glfwGetTime();
		//float greenValue = sin(timeValue) / 2.0f + 0.5f;

		float redvalue = 0;
		float greenvalue = 0;
		float bluevalue = 0;




		//ourShader.setFloat("red", redvalue);
		//ourShader.setFloat("green", greenvalue);
		//ourShader.setFloat("blue", bluevalue);


		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);


		//set_rowRGB(7,0.0f,0.0f,1.0f);
		//set_xhue(0, 0, 0, 1);

		
		set_allhue(huec++);
		//set_allRGB(1.0f, 0, 0);

		

		if (stepper == 512)stepper = 0;
		if (huec == 360)huec = 0;

















		// render boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 512; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, translations[i]);
			float angle = 1.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);




			//color the cube
			//if (i == ledloop) {
			//	redvalue = 1.0f;
			//}
			//else {
			//	redvalue = 0.0f;
			//}


			
			int rowi =  i /64;
			


			redvalue = display_array[rowi][i%64];
			greenvalue = display_array[rowi][i%64 + 64];
			bluevalue = display_array[rowi][i%64 + 128];




			ourShader.setFloat("red", redvalue);
			ourShader.setFloat("green", greenvalue);
			ourShader.setFloat("blue", bluevalue);


			//glBindVertexArray(quadVAO);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 512);



			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		//ledloop = ledloop += 1;
		//if (ledloop == 512) {
		//	ledloop = 0;
		//}


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