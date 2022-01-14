#pragma once

#include <iostream>

#include "Renderer.h"

// Change game name once decided upon
class GL
{
public:

	GL();

	void Run();

	~GL(void);

protected:

private:
	Renderer* m_renderer;
	int WinID = NULL;

};