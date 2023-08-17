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

//Create a shader with location and position of triangle
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
"}\n\0";

//Create a shader that will handle the colour and texture of the triangle
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.0f, 0.2f, 1.0f);\n"
"}\n\0";

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

#pragma region Build and compile our shader program
	// BUILD AND COMPILE OUR SHADER PROGRAM
	// Build our shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile our shader
	glCompileShader(vertexShader);

	// Compile time errors variables
	GLint success;
	GLchar infoLog[512];

	// Check for compile time errors and save the status in success variable
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// Check if the shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get compilation errors and save in infoLog variable
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	#pragma region Fragment shader program
		// Build our fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

		// Compile our fragment shader
		glCompileShader(fragmentShader);

		// Check for compile time errors and save the status in success variable
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		// Check if the shader has successfully compiled
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Get compilation errors and save in infoLog variable
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}
	#pragma endregion

		// Link the Shaders
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Check for linking errors and save in success variable
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		// Check if the shader has successfully linked
		if (!success)
		{
			glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog); // Get Linking errors and save in infoLog variable
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}

		// Shaders have linked successfully and we can now delete the individual shaders as they are linked in shaderProgram
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Set vertex data for our triangle
		GLfloat vertices[] =
		{
			//Positions
			0.5f, -0.5f, 0.0f,   // Left
			-0.5f, -0.5f, 0.0f,  // Right
			0.0f, 0.5f, 0.0f     // Top
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
		glClearColor(0.4f, 0.8f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAW OUR TRIANGLE
		glUseProgram(shaderProgram);
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