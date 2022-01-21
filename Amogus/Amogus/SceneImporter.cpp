#include "SceneImporter.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "JSONHelpers.h"
#include "ShaderFactory.h"
#include "source.h"

#include "Camera.h"
#include "Sprite.h"
#include "Transform.h"

extern Application* g_app;

namespace SceneImporter
{
	bool ReadAllComponents(const nlohmann::json& jComponents, Scene* scene);

	bool ReadAllEntities(const nlohmann::json& jEntityArray, EntityManager* entityManager);
	bool ReadComponentsOfEntity(const nlohmann::json& jComponentArray, Entity entity, EntityManager* entityManager);

	bool CreateCamera(const nlohmann::json& j, Entity entity, EntityManager* entityManager);
	bool CreateSprite(const nlohmann::json& j, Entity entity, EntityManager* entityManager);
	bool CreateTransform(const nlohmann::json& j, Entity entity, EntityManager* entityManager);

	bool ImportSceneFromFile(const std::string& filePath)
	{
		std::ifstream inFile(filePath);
		if (inFile.bad())
		{
			return false;
		}

		nlohmann::json jFile = nlohmann::json::parse(inFile);

		if (!jFile.contains("entities"))
		{
			return false;
		}

		std::string name = jFile["name"];

		glm::vec3 clearColour;
		if (!JSON::ReadVec3(clearColour, jFile["clearColour"])) {}

		SceneManager* sceneManager = g_app->m_sceneManager;

		sceneManager->CreateScene(name, clearColour);
		Scene* scene = sceneManager->GetScene(name);

		if (!ReadAllEntities(jFile["entities"], scene->m_entityManager))
		{
			return false;
		}

		return true;
	}

	bool ReadAllComponents(const nlohmann::json& jComponents, Scene* scene)
	{
		EntityManager* entityManager = scene->m_entityManager;

		bool success = true;

		//if(!ReadComponentsOfType<Camera>(jComponents))

		//if (!ReadAllCameras(jComponents["cameras"], entityManager)) { success = false; }
		//if (!ReadAllSprites(jComponents["sprites"], entityManager)) { success = false; }
		//if (!ReadAllTransforms(jComponents["transforms"], entityManager)) { success = false; }

		return success;
	}

	bool ReadAllEntities(const nlohmann::json& jEntityArray, EntityManager* entityManager)
	{
		bool success = true;

		for (int i = 0; i < jEntityArray.size(); i++)
		{
			Entity entity = entityManager->CreateEntity();

			if (!ReadComponentsOfEntity(jEntityArray[i], entity, entityManager))
			{
				success = false;
			}
		}

		return success;
	}

	bool ReadComponentsOfEntity(const nlohmann::json& jComponentArray, Entity entity, EntityManager* entityManager)
	{
		bool success = true;

		for (int i = 0; i < jComponentArray.size(); i++)
		{
			nlohmann::json jComponent = jComponentArray[i];

			if (jComponent["type"] == "camera")
			{
				if (!CreateCamera(jComponent, entity, entityManager))
				{
					success = false;
				}
			}
			else if (jComponent["type"] == "sprite")
			{
				if (!CreateSprite(jComponent, entity, entityManager))
				{
					success = false;
				}
			}
			else if (jComponent["type"] == "transform")
			{
				if (!CreateTransform(jComponent, entity, entityManager))
				{
					success = false;
				}
			}
		}

		return true;
	}

	bool CreateCamera(const nlohmann::json& j, Entity entity, EntityManager* entityManager)
	{
		bool success = true;

		float viewportWidth;
		if (!JSON::Read(viewportWidth, j["viewport"]["width"]))
		{
			success = false;
		}

		float viewportHeight;
		if (!JSON::Read(viewportHeight, j["viewport"]["height"]))
		{
			success = false;
		}

		float near, far;
		if (!JSON::Read(near, j["near"]) || !JSON::Read(far, j["far"])) // If near and far are not specified, use defaults
		{
			Camera* camera = entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight);
		}
		else
		{
			Camera* camera = entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight, near, far);
		}

		return success;
	}

	bool CreateSprite(const nlohmann::json& j, Entity entity, EntityManager* entityManager)
	{
		bool success = true;

		glm::vec3 colour;
		if (!JSON::ReadVec3(colour, j["colour"])) {}

		Shader* shader;
		{
			std::string vertexPath, fragmentPath;
			if (!JSON::Read(vertexPath, j["vertexFilePath"]) || !JSON::Read(fragmentPath, j["fragmentFilePath"]))
			{
				success = false; // Cannot render a sprite without vertex & fragment shader
			}

			std::string geometryPath;
			if (!JSON::Read(geometryPath, j["geometryFilePath"])) {}

			shader = ShaderFactory::CreatePipelineShader(vertexPath, fragmentPath, geometryPath);
		}

		std::string textureName;
		if (!JSON::Read(textureName, j["texture"]["name"])) {}

		std::string texturePath;
		if (!JSON::Read(texturePath, j["texture"]["filePath"]))
		{
			success = false; // Cannot render a sprite without a texture
		}

		Texture2D texture = TextureLoader::CreateTexture2DFromFile(textureName, texturePath);

		Sprite* component = entityManager->AddComponent<Sprite>(entity, texture, colour, shader);

		return success;
	}

	bool CreateTransform(const nlohmann::json& j, Entity entity, EntityManager* entityManager)
	{
		bool success = true;
		
		glm::vec2 pos;
		if (!JSON::ReadVec2(pos, j["pos"])) {}

		glm::vec2 size;
		if (!JSON::ReadVec2(size, j["size"])) {}

		float rotate;
		if (!JSON::Read(rotate, j["rotate"])) {}

		Transform* component = entityManager->AddComponent<Transform>(entity, pos, size, rotate);

		return success;
	}
}