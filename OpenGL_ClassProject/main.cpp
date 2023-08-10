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
	//initialise GLFW
	glfwInit();

	//Set all required options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create a GLFWindow object that can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyWindowName", nullptr, nullptr);

	return EXIT_SUCCESS;
}