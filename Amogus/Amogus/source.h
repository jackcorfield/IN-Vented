#pragma once

#include <iostream>

#include "EntityManager.h"
#include "SceneManager.h"
#include "AudioManager.h"

#include "Renderer.h"
#include "PhysicsSystem.h"


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
	AudioManager* m_audioManager;

protected:

private:
	bool InitGL();
	void InitImGui();
	void TerminateOpenGL();

	Renderer* m_renderer;
	PhysicsSystem* m_physicsSystem;
};