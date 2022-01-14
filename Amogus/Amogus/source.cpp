#include "source.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description);

GL::GL() 
{
	if (!InitGL())
	{
		glfwTerminate();
	}

	while (true)
	{

	}

	//Render();
	//Input();
}

bool GL::InitGL() 
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		// Failed to initialise GLFW
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Amogus Engine", NULL, NULL);
	if (!window)
	{
		// Failed to create window
		return false;
	}

	// Initialise glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Failed to initialise glad
		return false;
	}

	return true;
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void GL::Render() 
{
	
}

void GL::Input() 
{



}

void GL::Logging() 
{


}

GL::~GL(void) 
{

}