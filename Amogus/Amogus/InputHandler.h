#pragma once
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

protected:


private:

	std::fstream fs;

	// adjust these to not be fixed array with weird counts, for now just use ANSI standard for storing keycount
	std::string inputName[200];
	std::string inputID[200];
	std::string keyAction[200];

};

