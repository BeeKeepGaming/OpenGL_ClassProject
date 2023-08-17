#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//Link Shader File
#include "Shader.h"

//Window Dimention
const GLint WIDTH = 960, HEIGHT = 720;

int main()
{
#pragma region Initialisation of a window
	// Initialise GLFW
	glfwInit();

	//Set all required options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create a GLFWindow object that can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyWindowName", nullptr, nullptr);

	//Actual screen resolution
	int screenW, screenH;

	//Get screen resolution
	glfwGetFramebufferSize(window, &screenW, &screenH);

	//Check if window was created successfully
	if (nullptr == window) {
		cout << "Failed to create window." << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	//Making the window
	glfwMakeContextCurrent(window);

	//Enable GLEW
	glewExperimental = GL_TRUE;

	//Initialise GLEW
	if (GLEW_OK != glewInit()) {
		cout << "Failed to initialise GLEW." << endl;
		return EXIT_FAILURE;
	}

	//Setup OpenGL viewport
	//Define viewport dimentions
	glViewport(0, 0, screenW, screenH);

	// Enable alpha support for images ***
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Build & Compile Shader Program
	Shader ourShader("core.vs", "core.frag");

	// Set vertex data for our rectangle ***
	GLfloat vertices[] =
	{
		//Positions                //Colours
		0.5f, -0.5f, 0.0f,        1.0f, 0.0f, 0.0f,    //BOTTOM RIGHT
		-0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f,    //BOTTOM LEFT
		0.0f, 0.5f, 0.0f,         0.0f, 0.0f, 1.0f,    //TOP RIGHT
	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA, VOA;
	glGenVertexArrays(1, &VOA);
	glGenBuffers(1, &VBA);

	// Bind the vertex array object
	glBindVertexArray(VOA);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT))); //Colour
	glEnableVertexAttribArray(1);

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);
#pragma endregion

	//Game loop
	while (!glfwWindowShouldClose(window)) {
		//Checks for events and calls corresponding response
		glfwPollEvents();

		//Render
		//Clear the colour buffer
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAW OUR TRIANGLE
		ourShader.Use();
		glBindVertexArray(VOA);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); // Unbinding

		//Draw the OpenGL window/viewport
		glfwSwapBuffers(window);
	}

	// Properly deallocate all resources
	glDeleteVertexArrays(1, &VOA);
	glDeleteBuffers(1, &VBA);

	//Terminate GLFW and clear any resources from GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}