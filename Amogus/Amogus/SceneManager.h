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
	Scene* GetActiveScene() const;

	Scene* GetScene(const std::string& name) { return m_sceneMap[name]; }

	//std::string GetSceneName(Scene* scene) const { return std::find(m_sceneMap.begin(), m_sceneMap.end(), scene)->first;
	std::string GetSceneName(Scene* scene) const
	{
		auto res = std::find_if(m_sceneMap.begin(), m_sceneMap.end(), [&](const std::pair<std::string, Scene*>& param)
			{
				return param.second == scene;
			});
		if (res != m_sceneMap.end())
			return res->first;
		return "";
	}
	std::string GetActiveSceneName() const { return GetSceneName(m_activeScene); }

private:
	std::map<std::string, Scene*> m_sceneMap;
	Scene* m_activeScene;
};