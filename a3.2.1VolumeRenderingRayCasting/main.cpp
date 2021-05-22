#include <iostream>
#include <fstream>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void key_callback(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


//
GLuint initTFF1DTex(const char* filename);
GLuint initFace2DTex(GLuint texWidth, GLuint texHeight);
GLuint initVol3DTex(const char* filename, GLuint width, GLuint height, GLuint depth);
GLuint initFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight);
void drawBox(GLenum glFaces, GLuint vao);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
//camera.Zoom = 60.0f;
//camera.Up = glm.vec3(0.0f, 0.0f, -2.0f)
//camera.Front = glm::vec3(0.0f, 1.0f, 0.0f);
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float stepSize = 0.001f;

// The main function, from here we start the application and run the render loop
int main()
{
	// Init GLFW
	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Study OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	//glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader backFaceShader("./shaders/backface.vert", "./shaders/backface.frag");
	Shader RayCastingShader("./shaders/raycasting.vert", "./shaders/raycasting.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[24] = {
		// Positions 
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 1.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 1.0, 1.0
	};
	
	GLuint indices[36] = {
	   1, 5, 7,
	   7, 3, 1,
	   0, 2, 6,
	   6, 4, 0,
	   0, 1, 3,
	   3, 2, 0,
	   7, 5, 4,
	   4, 6, 7,
	   2, 3, 7,
	   7, 6, 2,
	   1, 0, 4,
	   4, 5, 1
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// color attribute(save position)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO
		

	// Load and create a texture 
	GLuint diffuseMap, specularMap;
	GLuint tffTexObj = initTFF1DTex("./resources/tff.dat");
	GLuint bfTexObj = initFace2DTex(WIDTH, HEIGHT);
	GLuint volTexObj = initVol3DTex("./resources/head256.raw", 256, 256, 225);
	GLuint frameBuffer = initFrameBuffer(bfTexObj, WIDTH, HEIGHT);


	glEnable(GL_DEPTH_TEST);
	
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		//glfwPollEvents();

		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		key_callback(window);

		// Render
		// Clear the color buffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Back Face render (get the position of back face)
		backFaceShader.Use();
		// Set M V P
		//glm::mat4 view = camera.GetViewMatrix();
		//glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 400.0f);
		//glm::mat4 model = glm::mat4(1.0f);
		//model *= glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//// to make the "head256.raw" i.e. the volume data stand up.
		//model *= glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//model *= glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)WIDTH / HEIGHT, 0.1f, 400.f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 model = glm::mat4(1.0f);
		model *= glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// to make the "head256.raw" i.e. the volume data stand up.
		model *= glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model *= glm::translate(model,glm::vec3(-0.5f, -0.5f, -0.5f));
		// notice the multiplication order: reverse order of transform
		glm::mat4 mvp = projection * view * model;
		GLuint mvpIdx = glGetUniformLocation(backFaceShader.Program, "MVP");
		if (mvpIdx >= 0)
		{
			glUniformMatrix4fv(mvpIdx, 1, GL_FALSE, &mvp[0][0]);
		}
		else
		{
			std::cerr << "can't get the MVP" << std::endl;
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
		drawBox(GL_FRONT, VAO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ray casting render
		RayCastingShader.Use();
		mvpIdx = glGetUniformLocation(RayCastingShader.Program, "MVP");
		if (mvpIdx >= 0)
		{
			glUniformMatrix4fv(mvpIdx, 1, GL_FALSE, &mvp[0][0]);
		}
		else
		{
			std::cerr << "can't get the MVP" << std::endl;
		}

		GLint screenSizeLoc = glGetUniformLocation(RayCastingShader.Program, "ScreenSize");
		if (screenSizeLoc >= 0)
		{
			glUniform2f(screenSizeLoc, (float)WIDTH, (float)HEIGHT);
		}
		else
		{
			std::cout << "ScreenSize is not bind to the uniform" << std::endl;
		}
		GLint stepSizeLoc = glGetUniformLocation(RayCastingShader.Program, "StepSize");
		if (stepSizeLoc >= 0)
		{
			glUniform1f(stepSizeLoc, stepSize);
		}
		else
		{
			std::cout << "StepSize is not bind to the uniform"	<< std::endl;
		}
		GLint transferFuncLoc = glGetUniformLocation(RayCastingShader.Program, "TransferFunc");
		if (transferFuncLoc >= 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, tffTexObj);
			glUniform1i(transferFuncLoc, 0);
		}
		else
		{
			std::cout << "TransferFunc is not bind to the uniform"	<< std::endl;
		}
		GLint backFaceLoc = glGetUniformLocation(RayCastingShader.Program, "ExitPoints");
		if (backFaceLoc >= 0)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, bfTexObj);
			glUniform1i(backFaceLoc, 1);
		}
		else
		{
			std::cout << "ExitPoints"<< "is not bind to the uniform" << std::endl;
		}
		GLint volumeLoc = glGetUniformLocation(RayCastingShader.Program, "VolumeTex");
		if (volumeLoc >= 0)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_3D, volTexObj);
			glUniform1i(volumeLoc, 2);
		}
		else
		{
			std::cout << "VolumeTex is not bind to the uniform" << std::endl;
		}
		drawBox(GL_BACK, VAO);
				
		// Swap the screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &EBO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

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

// initialize the 1 dimensional texture for transfer function
GLuint initTFF1DTex(const char* filename)
{
	// read in the user defined data of transfer function
	std::ifstream inFile(filename, std::ifstream::in);
	if (!inFile)
	{
		std::cerr << "Error opening file: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}

	const int MAX_CNT = 10000;
	GLubyte *tff = (GLubyte *)calloc(MAX_CNT, sizeof(GLubyte));
	inFile.read(reinterpret_cast<char *>(tff), MAX_CNT);
	if (inFile.eof())
	{
		size_t bytecnt = inFile.gcount();
		*(tff + bytecnt) = '\0';
		std::cout << "bytecnt " << bytecnt << std::endl;
	}
	else if (inFile.fail())
	{
		std::cout << filename << "read failed " << std::endl;
	}
	else
	{
		std::cout << filename << "is too large" << std::endl;
	}
	GLuint tff1DTex;
	glGenTextures(1, &tff1DTex);
	glBindTexture(GL_TEXTURE_1D, tff1DTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, tff);
	free(tff);
	return tff1DTex;
}

// init the 2D texture for render back face 'bf' stands for back face
GLuint initFace2DTex(GLuint bfTexWidth, GLuint bfTexHeight)
{
	GLuint backFace2DTex;
	glGenTextures(1, &backFace2DTex);
	glBindTexture(GL_TEXTURE_2D, backFace2DTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bfTexWidth, bfTexHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	return backFace2DTex;
}

// init 3D texture to store the volume data used for ray casting
GLuint initVol3DTex(const char* filename, GLuint w, GLuint h, GLuint d)
{
	FILE *fp;
	size_t size = w * h * d;
	GLubyte *data = new GLubyte[size];			  // 8bit
	if (!(fp = fopen(filename, "rb")))
	{
		std::cout << "Error: opening .raw file failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "OK: open .raw file success" << std::endl;
	}
	if (fread(data, sizeof(char), size, fp) != size)
	{
		std::cout << "Error: read .raw file failed" << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "OK: read .raw file success" << std::endl;
	}
	fclose(fp);

	GLuint volTexObj;
	glGenTextures(1, &volTexObj);
	// bind 3D texture target
	glBindTexture(GL_TEXTURE_3D, volTexObj);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	// pixel transfer happens here from client to OpenGL server
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);

	delete[]data;
	std::cout << "volume texture created" << std::endl;
	return volTexObj;
}

// initialize the frame buffer, the only frame buffer used in this program
GLuint initFrameBuffer(GLuint texObj, GLuint texWidth, GLuint texHeight)
{
	// create a depth buffer for our frame buffer
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);

	// attach the texture and the depth buffer to the frame buffer
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texObj, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	
	//checkFramebufferStatus
	GLenum complete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (complete != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "frame buffer is not complete" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void drawBox(GLenum glFaces, GLuint vao)
{
	glEnable(GL_CULL_FACE);
	glCullFace(glFaces);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(0);
}

