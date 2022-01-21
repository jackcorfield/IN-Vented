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
	void Quit(KeyInputEvent* e);
	bool InitGL();
  
	void TerminateOpenGL();

	bool m_quit;

	Renderer* m_renderer;
};