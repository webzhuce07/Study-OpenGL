#include <iostream>

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


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// camera
Camera camera(glm::vec3(-1.0f, 2.0f, 6.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// The main function, from here we start the application and run the render loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	Shader lightingShader("./shaders/material.vs", "./shaders/material.frag");
	Shader lightCubeShader("./shaders/lightcube.vs", "./shaders/lightcube.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions         //Normal
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	
	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);	

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	GLuint lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use corresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);

		// Set lights properties
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		// Set material properties
		GLint matAmbientLoc = glGetUniformLocation(lightingShader.Program, "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(lightingShader.Program, "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(lightingShader.Program, "material.specular");
		GLint matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess");
		glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 32.0f);

		// Create camera transformations
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lightCubeShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lightCubeShader.Program, "model");
		viewLoc = glGetUniformLocation(lightCubeShader.Program, "view");
		projLoc = glGetUniformLocation(lightCubeShader.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
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
