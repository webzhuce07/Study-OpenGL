#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "stdio.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

/// 引入GLFW库
#include <GLFW/glfw3.h>
#include <iostream>


int main(int argc, char** argv)
{

	if (!glfwInit())	// 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		return -1;
	}

	// We are rendering off-screen, but a window is still needed for the context
	// creation. There are hints that this is no longer needed in GL 3.3, but that
	// windows still wants it. So just in case.
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE); //dont show the window

	// Open a window and create its OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(100, 100, "Introduction", NULL, NULL);
	if (window == NULL) {
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	/*In your program code, somewhere just after the GL context is created (typically in
	  an initialization function), and before any OpenGL functions are called, include the
	  following code:*/
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/*fprintf(stderr, "Error initializing GLEW: %s\n",
			glewGetErrorString(err));*/
	}
	/*In order to support a wide range of systems, it is essential to be able to query for the
	  supported OpenGL and GLSL version of the current driver. It is quite simple to do so, and
	  there are two main functions involved: glGetString and glGetIntegerv.*/
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion =
		glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);

	/*The following is an example of how you might go about making use of some of GLM*/
	glm::vec4 position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0));
	glm::mat4 mv = view * model;
	glm::vec4 transformed = mv * position;

	system("pause");
}