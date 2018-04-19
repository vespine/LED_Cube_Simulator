
// ----------------------------------------------------------
// Includes
// ----------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int main() {
	if (glfwInit() == false) {
		//did not succeed
		fprintf(stderr, "GLFW failed to initialise.");
		return -1;
	}

	// 4 AA
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




	GLFWwindow* window;
	window = glfwCreateWindow(1280, 720, "My OPENGL", NULL, NULL);

	if (!window) {
		fprintf(stderr, "Window failed to create");
		glfwTerminate();
		return -1;
		
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK){
		fprintf(stderr, "GLEW failed to initialise");
		glfwTerminate();
		return -1;
	}


	GLuint vaoID = 6;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);



	static const GLfloat verts[] = {
		// x, y, z
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	//generate VBO
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);







	while (!glfwWindowShouldClose(window)) {
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


}