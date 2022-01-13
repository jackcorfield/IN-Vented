#pragma once

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


using namespace::std;

// Change game name once decided upon
class Init
{
public:

	Init(int argc, char* argv[]);

	~Init(void);

	void Render();

	void Input();

	void Logging();



protected:

private:



};