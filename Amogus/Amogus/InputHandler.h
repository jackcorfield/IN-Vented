#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

struct GLFWindow;

struct InputMap 
{

	// Implement mapping in to struct

};

class InputHandler
{
public:

	InputHandler();
	~InputHandler();

	void InputMapRead();
	void InputMapWrite();

	void MapInput();

	// The GLFW callback for key events.  Sends events to all KeyInput instances
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

private:

	std::fstream fs;

	// adjust these to not be fixed array with weird counts, for now just use ANSI standard for storing keycount
	std::string inputName[200];
	std::string inputID[200];
	std::string keyAction[200];


};
