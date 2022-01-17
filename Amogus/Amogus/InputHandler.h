#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>


struct GLFWindow;

class InputHandler
{
public:

	InputHandler();
	~InputHandler();

	// The GLFW callback for key events.  Sends events to all KeyInput instances
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

	static void CallAction(std::string key);


private:


};
