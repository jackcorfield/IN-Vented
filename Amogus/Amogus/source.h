#pragma once

#include <iostream>

#include "EntityManager.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "PhysicsSystem.h"
#include "SceneManager.h"

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
	SceneManager* m_sceneManager;

protected:

private:
	bool InitGL();

	void TerminateOpenGL(KeyInputEvent* e);
	void TerminateOpenGL();

	Renderer* m_renderer;
	PhysicsSystem* m_physicsSystem;
};