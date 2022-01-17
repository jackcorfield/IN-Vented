#include "InputHandler.h"
#include "source.h"

// Handler Will require JSON Parser to read KEYMAP.json data at start and load maps into memory, 
// Map file to read located at Data/Config/KEYMAP.json

extern Application* g_app;

InputHandler::InputHandler()
{

	InputMapRead();

}

InputHandler::~InputHandler() 
{

}

void InputHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	
	// This function will require a helper bool function to set iskeydown

	std::cout << "PRESSING: " << key << std::endl;
	// to test against the KEYMAP.json the "1" key should output 49 to console
}

void InputHandler::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "MOUSE POS: " << xpos << "," << ypos << std::endl;
}

void InputHandler::InputMapRead()
{
	// Very quick and dirty string manip until a proper JSON parser is implemented

	fs.open("Data/Config/KEYMAP.json");
	if (fs.fail() || fs.bad()) 
	{
		std::cout << "BAD KEYMAP FILE" << std::endl;
		return;
	}

	uint32_t mapIndex = 0;
	std::string str;

	while (getline(fs, str))
	{
		if (str.find("{") != std::string::npos) 
		{
			// Do Something
		}
		if (str.find("}") != std::string::npos) 
		{
			// Do Something
		}
		if (str.find("inputName") != std::string::npos) 
		{

			//std::cout << "inputName" << std::endl;

			std::string token = str.substr(str.find(":") + 3, std::string::npos); // Grab Data after Colon
			token = token.substr(0, token.find(",") - 1); // Snip End off data
			inputName[mapIndex] = token;

			std::cout << token << std::endl;

		}
		if (str.find("inputID") != std::string::npos) 
		{

			//std::cout << "inputID" << std::endl;

			std::string token = str.substr(str.find(":") + 3, std::string::npos);
			token = token.substr(0, token.find(",") - 1);
			inputID[mapIndex] = token;

			std::cout << token << std::endl;

		}
		if (str.find("keyAction") != std::string::npos) 
		{

			//std::cout << "keyAction" << std::endl;

			std::string token = str.substr(str.find(":") + 3, std::string::npos);
			token = token.substr(0, token.find(",") - 1);
			keyAction[mapIndex] = token;

			std::cout << token << std::endl;

			mapIndex++;

		}

		//std::cout << str << std::endl;

	}

	// Store map here for quick routing of keys to actions during callback

}

void InputHandler::InputMapWrite()
{



}

void InputHandler::MapInput()
{

	// Implement later with GUI
	// May implement earlier via commands to build a keymap for testing until GUI is built

}