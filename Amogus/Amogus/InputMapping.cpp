#include "InputMapping.h"
#include <iostream>
#include <fstream>

// Refactor fo style standards

InputMap InputMapping::InputMapRead(std::string filename)
{
	// Very quick and dirty string manip until a proper JSON parser is decided on

	// This should be rebuilt in format ACTION, BIND1, BIND2

	std::map<std::string, std::string> keyMap;
	InputMap keyMapData(keyMap);

	std::fstream fs;

	fs.open(filename);
	if (fs.fail() || fs.bad())
	{
		std::cout << "BAD KEYMAP FILE" << std::endl;
		return keyMapData;
	}

	std::string str;
	std::string inputID;
	std::string keyAction;

	while (getline(fs, str))
	{
		// Not required however can be helpful in locating what numerical keyIDs match what input
		//
		//if (str.find("inputName") != std::string::npos) 
		//{
		//
		//	std::string token = str.substr(str.find(":") + 3, std::string::npos); // Grab Data after Colon
		//	token = token.substr(0, token.find(",") - 1); // Snip End off data
		//	inputName[mapIndex] = token;
		//
		//	std::cout << token << std::endl;
		//
		//}

		if (str.find("inputID") != std::string::npos)
		{

			std::string token = str.substr(str.find(":") + 3, std::string::npos);
			token = token.substr(0, token.find(",") - 1);
			inputID = token;

			std::cout << inputID << std::endl;

		}
		if (str.find("keyAction") != std::string::npos)
		{

			std::string token = str.substr(str.find(":") + 3, std::string::npos);
			token = token.substr(0, token.length()-1);
			keyAction = token;

			std::cout << keyAction << std::endl;

		}

		keyMapData.MapInput(inputID, keyAction);
	}

	return keyMapData;
}

void InputMapping::InputMapWrite()
{
	/*std::fstream fs;

	fs.open(filename);
	if (fs.fail() || fs.bad())
	{
		std::cout << "BAD KEYMAP FILE" << std::endl;
		return keyMapData;
	}

	while (getline(fs, str))
	{
		if (str.find("inputID") != std::string::npos)
		{
			std::string token = str.substr(str.find(":") + 3, std::string::npos);
			token = keyMap
			token = token.substr(0, token.find(",") - 1);
			inputID = token;

			std::cout << inputID << std::endl;

		}

		if (str.find("keyAction") != std::string::npos)
		{

			std::string token = str.substr(str.find(":") + 3, std::string::npos);
			token = token.substr(0, token.length()-1);
			keyAction = token;

			std::cout << keyAction << std::endl;
		}

		keyMapData.MapInput(inputID, keyAction);
	}*/
	// reverse read to search for inputID's and then apply required action to them

}

