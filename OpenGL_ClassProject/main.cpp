#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//GLEW
#define GLEW_STATIC
#include "GL/glew.h"

//GLFW
#include "GLFW/glfw3.h"

// SOIL2
#include "SOIL2/SOIL2.h"

// Glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//Link Shader File
#include "Shader.h"

//Link Camera File
#include "Camera.h"

//Window Dimention
const GLint WIDTH = 960, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT; // Replace all screenW & screenH with these

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow* window, double xPos, double yPos); // Get mouse pos in order to hide it
void ProcessInput(GLFWwindow* window); // Move camera

// Set Initial values for camera ***
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024]; // Array of 1024 different types of keys
bool firstMouse = true; // Only handling one type of mouse, thus true
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Switch Cameras
bool camLocked = false;

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

	//Get screen resolution
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Check if window was created successfully
	if (nullptr == window) {
		cout << "Failed to create window." << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	//Making the window
	glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // Center cursor to window and Hide the cursor for a more immersive experience
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Enable GLEW
	glewExperimental = GL_TRUE;

	//Initialise GLEW
	if (GLEW_OK != glewInit()) {
		cout << "Failed to initialise GLEW." << endl;
		return EXIT_FAILURE;
	}

	//Setup OpenGL viewport
	//Define viewport dimentions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Enable depth in the project
	glEnable(GL_DEPTH_TEST);

	// Enable alpha support for images
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#pragma endregion

//#pragma region Build Shader - Cubes
//
//	//Build & Compile Shader Program
//	Shader ourShader("core.vs", "core.frag");
//
//	// Set vertex data for our rectangle
//	// Set vertex data for our cube ***
//	GLfloat vertices[] =
//	{
//		//Positions                //Texture Coords
//		/// Front Triangles
//		-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
//		0.5f, -0.5f, -0.5f,			1.0f, 1.0f,
//
//		0.5f, -0.5f, -0.5f,			1.0f, 1.0f,
//		0.5f, -0.5f, 0.5f,			0.0f, 1.0f,
//		-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
//		/// Right Triangles
//		0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
//		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
//		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
//
//		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
//		0.5f, 0.5f, 0.5f,			0.0f, 1.0f,
//		0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
//		/// Back Triangles
//		0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
//		0.5f, 0.5f, -0.5f,			1.0f, 0.0f,
//		-0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
//
//		-0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
//		-0.5f, 0.5f, 0.5f,			0.0f, 1.0f,
//		0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
//		/// Left Triangles
//		-0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
//		-0.5f, 0.5f, -0.5f,			1.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,
//
//		-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,
//		-0.5f, -0.5f, 0.5f,			0.0f, 1.0f,
//		-0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
//		/// Top Triangles
//		-0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
//		-0.5f, -0.5f, 0.5f,			1.0f, 0.0f,
//		0.5f, -0.5f, 0.5f,			1.0f, 1.0f,
//
//		0.5f, -0.5f, 0.5f,			1.0f, 1.0f,
//		0.5f, 0.5f, 0.5f,			0.0f, 1.0f,
//		-0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
//		/// Bottom Triangles
//		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
//		-0.5f, 0.5f, -0.5f,			1.0f, 0.0f,
//		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
//
//		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,			0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f
//	};
//
//    // Positions of different cubes
//    glm::vec3 cubePositions[] =
//    {
//        glm::vec3(0.0f, 0.0f, 0.0f),
//        glm::vec3(2.0f, 5.0f, -15.0f),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -2.0f, -12.3f),
//        glm::vec3(2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f, 3.0f, -7.5f),
//        glm::vec3(1.3f, -2.0f, -2.5f),
//        glm::vec3(1.5f, 2.0f, -2.5f),
//        glm::vec3(1.5f, 0.2f, -1.5f),
//        glm::vec3(-1.3f, 1.0f, -1.5f)
//    };
//
//	// Generate the vertex arrays and vertex buffers and save them into variables
//	GLuint VBA, VOA;
//	glGenVertexArrays(1, &VOA);
//	glGenBuffers(1, &VBA);
//
//	// Bind the vertex array object
//	glBindVertexArray(VOA);
//
//	// Bind and set the vertex buffers
//	glBindBuffer(GL_ARRAY_BUFFER, VBA);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	// Create the vertex pointer and enable the vertex array
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); //Position
//	glEnableVertexAttribArray(0);
//
//	// Texture coordinate attribute
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //Texture
//	glEnableVertexAttribArray(2);
//
//	// Unbind the vertex array to prevent strange bugs
//	glBindVertexArray(0);
//
//	#pragma region Texrure creation & loading
//		// Create and load texture
//		GLuint texture;
//		int width, height;
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//
//		// Set texture parameters
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//		// Set texture filtering
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// Actual texture loading code
//		unsigned char* image = SOIL_load_image("res/images/image1.png", &width, &height, 0, SOIL_LOAD_RGBA);
//
//		// Specify 2D texture image
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//
//		// Generate mipmaps
//		glGenerateMipmap(GL_TEXTURE_2D);
//		SOIL_free_image_data(image);
//		glBindTexture(GL_TEXTURE_2D, 0);
//
//	#pragma endregion
//#pragma endregion

#pragma region Build Shader - Height Map
        Shader shaderHM("coreHM.vs", "coreHM.frag");

        // Create and load texture replace it with your own image path.
        int widthHM, heightHM, nrChannelsHM;

        unsigned char* dataHM = SOIL_load_image("res/images/Hight_Map.png", &widthHM, &heightHM, &nrChannelsHM, 0);

        if(dataHM)
        {
            cout << "Loaded heightmap of size " << heightHM << " x " << widthHM << endl;
        }
        else
        {
            cout << "Failed to load texture" << endl;
        }

        // set up vertex data (and buffer(s)) and configure vertex attributes
        vector<GLfloat> verticesHM;
        GLfloat yScale = 0.1; //normalize the height map data and scale it to the desired height [0f : 64.0f]
        GLfloat yShift = 16.0f; // translate the elevations to our final desired range [-16.0f : 48.0f]
        int rez = 1;
        GLuint bytePerPixel = nrChannelsHM;

        for (int i = 0; i < heightHM; i++)
        {
            for (int j = 0; j < widthHM; j++)
            {
                unsigned char* pixelOffset = dataHM + (j + widthHM * i) * bytePerPixel;
                unsigned char y = pixelOffset[0];

                // vertex
                verticesHM.push_back(-heightHM / 2.0f + heightHM * i / (float)heightHM); // vx
                verticesHM.push_back((int)y * yScale - yShift); // vy
                verticesHM.push_back(-widthHM / 2.0f + widthHM * j / (float)widthHM); // vz
            }
        }
        cout << "Loaded " << verticesHM.size() / 3 << " vertices" << endl;
        SOIL_free_image_data(dataHM);

        vector<GLuint> indicesHM;
        for (int i = 0; i < heightHM - 1; i += rez)
        {
            for (int j = 0; j < widthHM; j += rez)
            {
                for (int k = 0; k < 2; k++)
                {
                    indicesHM.push_back(j + widthHM * (i + k * rez));
                }
            }
        }
        cout << "Loaded " << indicesHM.size() << " indices" << endl;

        const int numStrips = (heightHM - 1) / rez;
        const int numTrisPerStrip = (widthHM / rez) * 2 - 2;
        cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << endl;
        cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << endl;

        // Generate the vertex arrays, vertex buffers and index buffers and save them into variables
        unsigned int VAO, VBO, IBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesHM.size() * sizeof(float), &verticesHM[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesHM.size() * sizeof(unsigned), &indicesHM[0], GL_STATIC_DRAW);

#pragma endregion

#pragma region BUILD AND COMPILE SHADER - CHESSBOARD
        Shader chessboardShader("coreCB.vs", "coreCB.frag");

        // Set vertex data for our cube
        GLfloat verticesBoard[] =
        {
            //Positions                //Texture Coords
            -0.5f, 0.0f, -0.5f,         0.0f, 0.0f,
            0.5f, 0.0f, -0.5f,          1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,         1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,         1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,
            -0.5f, 0.0f, -0.5f,         0.0f, 0.0f, // Left

            -0.5f, 0.0f,  0.5f,         0.0f, 0.0f,
            0.5f, 0.0f,  0.5f,          1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,         1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,         1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,        0.0f, 1.0f,
            -0.5f, 0.0f,  0.5f,         0.0f, 0.0f, // Right

            -0.5f,  0.5f,  0.5f,        1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,        1.0f, 1.0f,
            -0.5f, 0.0f, -0.5f,         0.0f, 1.0f,
            -0.5f, 0.0f, -0.5f,         0.0f, 1.0f,
            -0.5f, 0.0f,  0.5f,         0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,        1.0f, 0.0f, // Back

            0.5f,  0.5f,  0.5f,         1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,         1.0f, 1.0f,
            0.5f, 0.0f, -0.5f,          0.0f, 1.0f,
            0.5f, 0.0f, -0.5f,          0.0f, 1.0f,
            0.5f, 0.0f,  0.5f,          0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,         1.0f, 0.0f, // Front

            -0.5f, 0.0f, -0.5f,         0.0f, 1.0f,
            0.5f, 0.0f, -0.5f,          1.0f, 1.0f,
            0.5f, 0.0f,  0.5f,          1.0f, 0.0f,
            0.5f, 0.0f,  0.5f,          1.0f, 0.0f,
            -0.5f, 0.0f,  0.5f,         0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f,         0.0f, 1.0f, // Bottom

            -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,         1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,         1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,         1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,        0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,        0.0f, 1.0f // Top
        };

        // Positions of different cubes
        glm::vec3 cubePositions[] =
        {
            //float randomVal = (float)rand() / (RAND_MAX); // Generate a random value between 0 & 1
            // Row 1 Top
            glm::vec3(-3.0f, 0.0f, 4.0f),
            glm::vec3(-2.0f, 0.0f, 4.0f),
            glm::vec3(-1.0f, 0.0f, 4.0f),
            glm::vec3(0.0f, 0.0f, 4.0f),
            glm::vec3(1.0f, 0.0f, 4.0f),
            glm::vec3(2.0f, 0.0f, 4.0f),
            glm::vec3(3.0f, 0.0f, 4.0f),
            glm::vec3(4.0f, 0.0f, 4.0f)
        };

        // Generate the vertex arrays and vertex buffers and save them into variables
        GLuint VBA_Board, VOA_Board;
        glGenVertexArrays(1, &VOA_Board);
        glGenBuffers(1, &VBA_Board);

        // Bind the vertex array object
        glBindVertexArray(VOA_Board);

        // Bind and set the vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBA_Board);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBoard), verticesBoard, GL_STATIC_DRAW);

        // Create the vertex pointer and enable the vertex array
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); //Position
        glEnableVertexAttribArray(0);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 *sizeof(GLfloat))); //Texture
        glEnableVertexAttribArray(2);

        // Unbind the vertex array to prevent strange bugs
        glBindVertexArray(0);

    #pragma region Chessboard Textures
        // Chessboard texture variables
        GLuint textureWhite, textureBlack;
        int widthB, heightB;

        #pragma region White Texture
        // Create and load White texture
        glGenTextures(1, &textureWhite);
        glBindTexture(GL_TEXTURE_2D, textureWhite);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Actual texture loading code
        unsigned char* whiteBlock = SOIL_load_image("res/textures/white.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

        // Specify 2D texture image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, whiteBlock);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(whiteBlock);
        glBindTexture(GL_TEXTURE_2D, 0);
        #pragma endregion
        #pragma region Black Texture
        // Create and load Black texture
        glGenTextures(1, &textureBlack);
        glBindTexture(GL_TEXTURE_2D, textureBlack);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Actual texture loading code
        unsigned char* blackBlock = SOIL_load_image("res/textures/black.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

        // Specify 2D texture image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, blackBlock);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(blackBlock);
        glBindTexture(GL_TEXTURE_2D, 0);

        #pragma endregion

    #pragma endregion

#pragma endregion


	//Game loop
	while (!glfwWindowShouldClose(window)) {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		//Checks for events and calls corresponding response
		glfwPollEvents();

        //Handle the movement 
        ProcessInput(window);

		//Render
		//Clear the colour buffer
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//#pragma region Draw Cube
//
//		// DRAW OUR CUBE		
//		ourShader.Use();
//
//		// Activate specified texture
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
//
//        // Create Projection Matrix - (moved into while loop in order to update zoom)
//        glm::mat4 projection(1.0f);
//        //Perspective view
//        projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
//
//		/// Create camera transformation
//        glm::mat4 view(1.0f);
//        view = camera.GetViewMatrix();
//
//        // Get the uniform locations for our matrices
//        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
//        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
//        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
//
//        // Pass locations to shaders
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//		// Draw container
//		glBindVertexArray(VOA);
//		
//        for (GLuint i = 0; i < 10; i++)
//        {
//            // Calculate the model matrix for each object and pass it to the shader before drawing
//            glm::mat4 model(1.0f);
//            model = glm::translate(model, cubePositions[i]);
//            GLfloat angle = 20.0f * i;
//            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }
//
//        glBindVertexArray(0); // Unbinding
//#pragma endregion

#pragma region DRAW CHESSBOARD
        // Activate Shader
        chessboardShader.Use();

        // Create Projection Matrix
        glm::mat4 projection_Board(1.0f);
        //Perspective view 
        projection_Board = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

        // Create camera transformation 
        glm::mat4 view_Board(1.0f);
        view_Board = camera.GetViewMatrix();

        // Get the uniform locations for our matrices
        GLint modelLoc_Board = glGetUniformLocation(chessboardShader.Program, "model");
        GLint viewLoc_Board = glGetUniformLocation(chessboardShader.Program, "view");
        GLint projLoc_Board = glGetUniformLocation(chessboardShader.Program, "projection");

        // Pass locations to shaders
        glUniformMatrix4fv(viewLoc_Board, 1, GL_FALSE, glm::value_ptr(view_Board));
        glUniformMatrix4fv(projLoc_Board, 1, GL_FALSE, glm::value_ptr(projection_Board));

        // Draw container
        glBindVertexArray(VOA_Board);

        for (GLuint i = 0; i < 8; i++)
        {
            for (GLuint j = 0; j < 8; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    // Activate White texture
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, textureWhite);
                    glUniform1i(glGetUniformLocation(chessboardShader.Program, "ourTexture1"), 0);
                }
                else
                {
                    // Activate Black texture
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, textureBlack);
                    glUniform1i(glGetUniformLocation(chessboardShader.Program, "ourTexture1"), 0);
                }

                // Calculate the model matrix for each object and pass it to the shader before drawing
                glm::mat4 model_Board(1.0f);
                glm::vec3 cubePos(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z - j);
                model_Board = glm::translate(model_Board, cubePos);
                GLfloat angle = 0.0f;
                model_Board = glm::rotate(model_Board, angle, glm::vec3(1.0f, 0.0f, 0.0f));
                glUniformMatrix4fv(modelLoc_Board, 1, GL_FALSE, glm::value_ptr(model_Board));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
#pragma endregion

#pragma region Draw HightMap
        // Activate Shader
        shaderHM.Use();

        // view/projection transformations
        glm::mat4 projectionHM = glm::perspective(glm::radians(camera.GetZoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100000.0f);
        glm::mat4 viewHM = camera.GetViewMatrix();
        GLint projLocHM = glGetUniformLocation(shaderHM.Program, "projection");
        GLint viewLocHM = glGetUniformLocation(shaderHM.Program, "view");

        glUniformMatrix4fv(viewLocHM, 1, GL_FALSE, glm::value_ptr(viewHM));
        glUniformMatrix4fv(projLocHM, 1, GL_FALSE, glm::value_ptr(projectionHM));

        // world transformation
        glm::mat4 modelHM = glm::mat4(1.0f);
        GLint modelLocHM = glGetUniformLocation(shaderHM.Program, "model");
        glUniformMatrix4fv(modelLocHM, 1, GL_FALSE, glm::value_ptr(modelHM));

        // Draw container
        glBindVertexArray(VAO);

        for (int strip = 0; strip < numStrips; strip++)
        {
            glDrawElements(GL_TRIANGLE_STRIP, // primitive type
                numTrisPerStrip + 2, // number of indices to render
                GL_UNSIGNED_INT, // index data type
                (void*)(sizeof(GLuint) * (numTrisPerStrip + 2) * strip)); // offset to starting index
        }

        glBindVertexArray(0); // Unbinding


#pragma endregion


		//Draw the OpenGL window/viewport
		glfwSwapBuffers(window);
	}

	// Properly deallocate all resources
	/*glDeleteVertexArrays(1, &VOA);
	glDeleteBuffers(1, &VBA);*/
	
	//Terminate GLFW and clear any resources from GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
// WASD and Arrow keys
void ProcessInput(GLFWwindow* window)
{
    //Free Camera controls
    if (!camLocked) {
        if (keys[GLFW_KEY_W])
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }

        if (keys[GLFW_KEY_S])
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }

        if (keys[GLFW_KEY_A])
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }

        if (keys[GLFW_KEY_D])
        {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        if (camLocked == true)
        {
            camLocked = false;
        }
        else
        {
            camLocked = true;
        }
    }

    //Cycle Camera Left
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        camLocked = true;
        camera.CycleCamera("Left");
    }

    //Cycle Camera Right
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        camLocked = true;
        camera.CycleCamera("Right");
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

// GLFW: whenever the mouse moves, this callback is called
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (!camLocked) {
        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        GLfloat xOffset = xPos - lastX;
        GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

        lastX = xPos;
        lastY = yPos;

        camera.ProcessMouseMovement(xOffset, yOffset);
    }
}

// GLFW: whenever the mouse scroll wheel scrolls, this callback is called
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    if (!camLocked) {
        camera.ProcessMouseScroll(yOffset);
    }
}