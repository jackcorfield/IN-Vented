#include "SceneManager.h"

#include <fstream>
#include <nlohmann/json.hpp>

//#include "JSONHelpers.h"

#include "Camera.h"
#include "ShaderFactory.h"
#include "Sprite.h"
#include "Transform.h"

//bool ReadCameraComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager);
//bool ReadSpriteComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager);
//bool ReadTransformComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager);

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
}

Scene* SceneManager::GetActiveScene()
{
	return m_activeScene;
}

//bool ReadCameraComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
//{
//	float viewportWidth, viewportHeight;
//	if (!JSON::Read(viewportWidth, j["viewport"]["width"]) || !JSON::Read(viewportHeight, j["viewport"]["height"]))
//	{
//		return false; // Camera needs a viewport width and height
//	}
//
//	float near, far;
//	if (!JSON::Read(near, j["near"]) || !JSON::Read(far, j["far"])) // If near and far are not specified, use defaults
//	{
//		Camera* component = entityManager->AddComponent<Camera>(e, viewportWidth, viewportHeight);
//
//		return true;
//	}
//
//	Camera* component = entityManager->AddComponent<Camera>(e, viewportWidth, viewportHeight, near, far);
//
//	return true;
//}
//
//bool ReadSpriteComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
//{
//	glm::vec3 colour;
//	if (!JSON::ReadVec3(colour, j["colour"]))
//	{}
//
//	Shader* shader;
//	{
//		std::string vertexPath, fragmentPath, geometryPath;
//		if (!JSON::Read(vertexPath, j["vertexFilePath"]) || !JSON::Read(fragmentPath, j["fragmentFilePath"]))
//		{
//			return false; // Cannot render a sprite without vertex & fragment shader
//		}
//		if (!JSON::Read(geometryPath, j["geometryFilePath"]))
//		{
//		}
//		shader = ShaderFactory::CreatePipelineShader(vertexPath, fragmentPath, geometryPath);
//	}
//
//	std::string textureName;
//	if (!JSON::Read(textureName, j["texture"]["name"]))
//	{}
//
//	std::string texturePath;
//	if (!JSON::Read(texturePath, j["texture"]["filePath"]))
//	{
//		return false; // Cannot render a sprite without a texture
//	}
//
//	Texture2D texture = TextureLoader::CreateTexture2DFromFile(textureName, texturePath);
//
//	Sprite* component = entityManager->AddComponent<Sprite>(e, texture, colour, shader);
//
//	return true;
//}

//bool ReadTransformComponent(const nlohmann::json& j, const Entity e, EntityManager* entityManager)
//{
//	glm::vec2 pos;
//	if (!JSON::ReadVec2(pos, j["pos"])) {}
//
//	glm::vec2 size;
//	if (!JSON::ReadVec2(size, j["size"])) {}
//
//	float rotate;
//	if (!JSON::Read(rotate, j["rotate"])) {}
//
//	Transform* component = entityManager->AddComponent<Transform>(e, pos, size, rotate);
//
//	return true;
//}
//
//bool WriteCameraComponent(nlohmann::json& j, const Camera* component)
//{
//	const float viewportWidth = component->m_viewportWidth;
//	const float viewportHeight = component->m_viewportHeight;
//	if (!JSON::Write(viewportWidth, j["viewport"]["width"]) || !JSON::Write(viewportHeight, j["viewport"]["height"]))
//	{
//		return false; // Camera needs a viewport width and height
//	}
//
//	const float near = component->m_near;
//	if (!JSON::Write(near, j["near"])) {}
//
//	const float far = component->m_far;
//	if (!JSON::Write(far, j["far"])) {}
//
//	return true;
//}
//
//bool WriteSpriteComponent(nlohmann::json& j, const Sprite* component)
//{
//	const glm::vec3 colour = glm::vec3(0.0f);
//	if (!JSON::WriteVec3(colour, j["colour"])) {}
//
//	//const std::string vertexPath = component->GetShader()->GetVertexPath();
//	//const std::string fragmentPath = component->GetShader()->GetFragmentPath();
//	//if (!JSON::Write(vertexPath, j["vertexFilePath"]) || !JSON::Write(fragmentPath, j["fragmentFilePath"]))
//	//{
//		//return false; // Cannot render a sprite without vertex & fragment shader
//	//}
//
//	//const std::string geometryPath = component->GetShader()->GetGeometryPath();
//	//if (!geometryPath.empty())
//	//{
//	//	if (!JSON::Write(geometryPath, j["geometryFilePath"])) {}
//	//}
//
//	// const std::string textureName = component->GetTexture().m_name;
//	//if (!JSON::Write(textureName, j["texture"]["name"])) {}
//
//	// const std::string texturePath = component->GetTexture().m_filePath;
//	//if (!JSON::Write(texturePath, j["texture"]["filePath"]))
//	//{
//	//	return false; // Cannot render a sprite without a texture
//	//}
//
//	return true;
//}
//
//bool WriteTransformComponent(nlohmann::json& j, const Transform* component)
//{
//	const glm::vec2 pos = component->m_position;
//	if (!JSON::WriteVec2(pos, j["pos"])) {}
//
//	const glm::vec2 size = component->m_size;
//	if (!JSON::WriteVec2(size, j["size"])) {}
//
//	const float rotate = component->m_rotate;
//	if (!JSON::Write(rotate, j["rotate"])) {}
//
//	return true;
//}