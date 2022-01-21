#include "SceneManager.h"

SceneManager::SceneManager()
	: m_activeScene(nullptr)
{

}

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

void SceneManager::SetActiveScene(const std::string& name)
{
	m_activeScene = m_sceneMap[name];
}

Scene* SceneManager::GetActiveScene()
{
	return m_activeScene;
}
