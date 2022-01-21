#pragma once

#include <iostream>

#include "EntityManager.h"
#include "Renderer.h"

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

	EntityManager* m_entityManager;
protected:

private:
	bool InitGL();
	void TerminateOpenGL();

	Renderer* m_renderer;
};