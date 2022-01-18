#include "InputHandler.h"
#include "InputMapping.h"
#include "source.h"

InputMap* loadedMap;

enum Actions {
	quit
};

InputHandler::InputHandler()
{
	// Map file to read located at Data/Config/KEYMAP.json
	loadedMap = new InputMap(InputMapping::InputMapRead("Data/Config/KEYMAP.json"));
}

InputHandler::~InputHandler() 
{
}

void InputHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	
	// This function will require a helper bool function to set iskeydown

	std::cout << "PRESSING: " << key << std::endl;

	// Convert to string to find in stored keymap
	std::string tempKey = std::to_string(key);

	CallAction(tempKey);

	// to test against the KEYMAP.json the "F" key should output PayRespects to console
}

void InputHandler::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "MOUSE POS: " << xpos << "," << ypos << std::endl;
}

void InputHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// This function will require a helper bool function to set iskeydown

	std::cout << "PRESSING: " << button << std::endl;

	// Convert to string to find in stored keymap
	std::string tempButton = std::to_string(button);

	CallAction(tempButton);
}

void InputHandler::CallAction(std::string input) 
{
	std::string mapping = (loadedMap->keyMap.find(input) != loadedMap->keyMap.end() ? loadedMap->keyMap.find(input)->second : input);

	//std::cout << mapping << std::endl;

	if (mapping == "quit") {
		exit(0);

	}

}

