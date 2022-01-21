#pragma once

#include <iostream>

#include "Renderer.h"
#include "InputHandler.h"

struct GLFWwindow;

struct WindowParams
{
	unsigned int windowWidth, windowHeight;
	bool isFullscreen;
	unsigned int MSAASamples;
	const char* windowTitle;
};

// Change game name once decided upon
class Application
{
public:

	Application();
	~Application();

	void Init();
	void Run();

	GLFWwindow* m_window;
	WindowParams m_windowParams;

protected:

private:
	bool InitGL();
	void TerminateOpenGL(KeyInputEvent* e);

	Renderer* m_renderer;
};