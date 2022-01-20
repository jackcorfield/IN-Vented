// Refactor fo style standards

#include "InputHandler.h"
#include "InputMapping.h"
#include "source.h"
#include "EventHandler.h"

InputMap* loadedMap;

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

	std::string tempKey = std::to_string(key);	// Convert to string to find in stored keymap

	CallAction(tempKey);

}

void InputHandler::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Do something
}

void InputHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// This function will require a helper bool function to set iskeydown

	std::string tempButton = std::to_string(button);	// Convert to string to find in stored keymap

	CallAction(tempButton);

}

// Add in callbacks for joy etc

void InputHandler::CallAction(std::string input) 
{
	std::string mapping = (loadedMap->keyMap.find(input) != loadedMap->keyMap.end() ? loadedMap->keyMap.find(input)->second : input);

	// Implement eventBus system here

}

