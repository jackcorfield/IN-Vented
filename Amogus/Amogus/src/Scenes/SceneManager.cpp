#include "SceneManager.h"

#include <fstream>
#include <nlohmann/include/nlohmann/json.hpp>

#include <ResourceFactories/ShaderFactory.h>

#include <Core/source.h>

#include <ECS/Components/Camera.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/Transform.h>

extern Application* g_app;

SceneManager::SceneManager()
	: m_activeScene(nullptr)
{}

SceneManager::~SceneManager()
{
	m_sceneMap.clear();
}

bool SceneManager::CreateScene(const std::string& name, const glm::vec3& sceneColour)
{
	if (SceneExists(name))
		return false;

	m_sceneMap[name] = new Scene(sceneColour);

	if (m_activeScene == nullptr)
		m_activeScene = m_sceneMap[name];

	return true;
}

bool SceneManager::DestroyScene(const std::string& name)
{
	if (SceneExists(name))
	{
		delete m_sceneMap[name];
		m_sceneMap.erase(name);
		return true;
	}
	return false;
}

bool SceneManager::SceneExists(const std::string& name)
{
	return (m_sceneMap.count(name) == 1);
}

bool SceneManager::ReadSceneFromFile(const std::string& filePath)
{

	return true;
}

bool SceneManager::WriteActiveSceneToFile(const std::string& filePath)
{

	return true;
}

void SceneManager::SetActiveScene(const std::string& name)
{
	m_activeScene = m_sceneMap[name];

	std::string windowTitle = "Editor: " + name;
	glfwSetWindowTitle(g_app->m_window, windowTitle.c_str());
	
}

Scene* SceneManager::GetActiveScene() const
{
	return m_activeScene;
}