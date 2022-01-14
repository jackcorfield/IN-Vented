#include "source.h"

#include <GLFW/glfw3.h>

GL::GL() :
	m_renderer(nullptr)
{
	WindowParams wParams =
	{
		1280, 720,
		false,
		0,
		"Engine"
	};

	m_renderer = new Renderer(wParams);

	//Render();
	//Input();
}

void GL::Run()
{
	bool quit = false;

	while (!quit)
	{
		glfwPollEvents();
		m_renderer->Render();
	}
}

GL::~GL(void) 
{

}