#pragma once

#include <glm.hpp>
#include <map>
#include <string>

#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool CreateScene(const std::string& name, const glm::vec3& sceneColour = glm::vec3(0.0f));
	bool DestroyScene(const std::string& name);
	bool SceneExists(const std::string& name);

	bool ReadSceneFromFile(const std::string& filePath);
	bool WriteActiveSceneToFile(const std::string& filePath);

	void SetActiveScene(const std::string& name);
	Scene* GetActiveScene();

	Scene* GetScene(const std::string& name) { return m_sceneMap[name]; }

private:
	std::map<std::string, Scene*> m_sceneMap;
	Scene* m_activeScene;
};