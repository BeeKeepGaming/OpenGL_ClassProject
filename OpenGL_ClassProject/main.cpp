#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

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

	//Game loop
	while (!glfwWindowShouldClose(window)) {
		//Checks for events and calls corresponding response
		glfwPollEvents();

		//Render
		//Clear the colour buffer
		glClearColor(0.4f, 0.8f, 0.4f, 1.0f);
		//6.0f, 226.0f, 109.0f
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw the OpenGL window/viewport
		glfwSwapBuffers(window);
	}

	//Terminate GLFW and clear any resources from GLFW
	glfwTerminate();

	#pragma endregion

	return EXIT_SUCCESS;
}