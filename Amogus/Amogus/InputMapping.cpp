#include "InputMapping.h"
#include <iostream>
#include <fstream>

InputMap InputMapping::InputMapRead(std::string filename)
{
	// Very quick and dirty string manip until a proper JSON parser is decided on

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
			token = token.substr(0, token.find(",") - 1);
			keyAction = token;

			std::cout << keyAction << std::endl;



		}
		keyMapData.MapInput(inputID, keyAction);
	}

	return keyMapData;
}

void InputMapping::InputMapWrite()
{



}

