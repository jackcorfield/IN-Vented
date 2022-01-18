#include "SceneManager.h"

#include <nlohmann/json.hpp>

#include "JSONHelpers.h"

#include "Camera.h"
#include "ShaderFactory.h"
#include "Sprite.h"
#include "Transform.h"

bool ReadEntities(const nlohmann::json& j, Scene* scene);
bool ReadComponentsOfEntity(const nlohmann::json& j, const Entity e, EntityManager* entityManager);

bool ReadCameraComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager);
bool ReadSpriteComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager);
bool ReadTransformComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager);

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

bool SceneManager::LoadScene(const std::string& filePath)
{
}

void SceneManager::SetActiveScene(const std::string& name)
{
	m_activeScene = m_sceneMap[name];
}

Scene* SceneManager::GetActiveScene()
{
	return m_activeScene;
}

bool ReadEntities(const nlohmann::json& j, Scene* scene)
{
	EntityManager* entityManager = scene->m_entityManager;

	for (int i = 0; i < j.size(); i++)
	{
		Entity entity = entityManager->CreateEntity();

		if (!ReadComponents(j, entity, entityManager))
		{
			return false;
		}
	}
}

bool ReadComponents(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
{
	for (int i = 0; i < j.size(); i++)
	{
		nlohmann::json jComponent = j[i];

		if (jComponent["type"] == "camera")
		{
			if (!ReadCameraComponent(j, e, entityManager))
			{
				return false;
			}
		}
		else if (jComponent["type"] == "sprite")
		{
			if (!ReadSpriteComponent(j, e, entityManager))
			{
				return false;
			}
		}
		else if (jComponent["type"] == "transform")
		{
			if (!ReadTransformComponent(j, e, entityManager))
			{
				return false;
			}
		}
	}
}

bool ReadCameraComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
{
	float viewportWidth, float viewportHeight;
	if (!JSON::Read("width", j["viewport"]) || !JSON::Read("height", j["viewport"]))
	{
		return false;
	}

	float near, far;
	if (!JSON::Read(near, j["near"]) || !JSON::Read(far, j["far"])) // If near and far are not specified, use defaults
	{
		Camera* component = entityManager->AddComponent<Camera>(e, viewportWidth, viewportHeight);

		return true;
	}

	Camera* component = entityManager->AddComponent<Camera>(e, viewportWidth, viewportHeight, near, far);

	return true;
}

bool ReadSpriteComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
{
	glm::vec3 colour;
	if (!JSON::ReadVec3(colour, j["colour"]))
	{}

	Shader* shader;
	{
		std::string vertexPath, fragmentPath, geometryPath;
		if (!JSON::Read(vertexPath, j["vertexFilePath"]) || !JSON::Read(fragmentPath, j["fragmentFilePath"]))
		{
			return false; // Cannot render a sprite without vertex & fragment shader
		}
		if (!JSON::Read(geometryPath, j["geometryFilePath"]))
		{
		}
		shader = ShaderFactory::CreatePipelineShader(vertexPath, fragmentPath, geometryPath);
	}

	std::string name;
	if (!JSON::Read(name, j["texture"]["name"]))
	{}

	std::string texturePath;
	if (!JSON::Read(texturePath, j["texture"]["filePath"]))
	{
		return false; // Cannot render a sprite without a texture
	}

	Texture2D texture = TextureLoader::CreateTexture2DFromFile(name, texturePath);

	Sprite* component = entityManager->AddComponent<Sprite>(e, texture, colour, shader);
}

bool ReadTransformComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
{
	glm::vec2 pos;
	if (!JSON::ReadVec2(pos, j["pos"]))
	{}

	glm::vec2 size;
	if (!JSON::ReadVec2(size, j["size"]))
	{}

	float rotate;
	if (!JSON::Read(rotate, j["rotate"]))
	{}

	Transform* component = entityManager->AddComponent<Transform>(e, pos, size, rotate);
}