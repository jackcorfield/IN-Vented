#pragma once

#include <iostream>

#include <ECS/EntityManager.h>
#include <Scenes/SceneManager.h>
#include <ECS/Systems/AudioManager.h>
#include <ECS/Systems/CollisionManager.h>

#include <ECS/Systems/Renderer.h>
#include <Handlers/InputHandler.h>
#include <ECS/Systems/PhysicsSystem.h>
#include <ECS/Systems/ScriptSystem.h>


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

	void SetActiveCamera(Entity cameraEntity) { m_renderer->SetActiveCamera(cameraEntity); }

	void Quit();

	GLFWwindow* m_window;
	WindowParams m_windowParams;

	EntityManager*		m_entityManager;
	SceneManager*		m_sceneManager;
	AudioManager*		m_audioManager;
	ScriptSystem*		m_scriptSystem;

	virtual void onInit() {};
	virtual void onUpdate(float dt) {}
	virtual void onRender(float dt) {}
	virtual void onImGui() {}
	virtual void onQuit() {}

protected:

	bool ImportScene(const char* sceneName, bool setActive = false);
	bool ExportScene(const char* sceneName);
	Renderer* m_renderer;

private:
	bool InitGL();
	void Quit(InputEvent* e);
	void TerminateOpenGL();
	bool m_quit;
};

Application* CreateApplication();