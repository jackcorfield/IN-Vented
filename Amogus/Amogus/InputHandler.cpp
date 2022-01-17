#include "InputHandler.h"
#include "InputMapping.h"
#include "source.h"

InputMap* loadedMap;

InputHandler::InputHandler()
{
	// Map file to read located at Data/Config/KEYMAP.json
	*loadedMap = InputMapping::InputMapRead("Data/Config/KEYMAP.json");
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

	// to test against the KEYMAP.json the "1" key should output 49 to console
}

void InputHandler::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "MOUSE POS: " << xpos << "," << ypos << std::endl;
}
void InputHandler::CallAction(std::string key) 
{
	std::string mapping = (*loadedMap->keyMap.find(key) != *loadedMap->keyMap.end() ? loadedMap->keyMap.find(key)->second : key);

	std::cout << mapping << std::endl;

}

