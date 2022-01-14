#pragma once

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace::std;

// Change game name once decided upon
class GL
{
public:

	GL();

	~GL(void);

	void Render();

	void Input();

	void Logging();

	bool InitGL();


protected:

private:

	int WinID = NULL;

};