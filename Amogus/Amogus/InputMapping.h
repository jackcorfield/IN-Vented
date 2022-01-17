#pragma once

#include <string>
#include <map>

struct InputMap
{
	InputMap(std::map<std::string, std::string> keyMap) : keyMap(keyMap) {}

	void MapInput(std::string inputID, std::string keyAction) {
		keyMap.erase(inputID);
		keyMap.insert(std::map<std::string, std::string>::value_type(inputID, keyAction));
	}

	std::map<std::string, std::string> keyMap;

};

class InputMapping
{
public:


	static InputMap InputMapRead(std::string filename);
	void InputMapWrite();


private:



};

