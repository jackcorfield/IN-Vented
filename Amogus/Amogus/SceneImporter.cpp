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
	bool ReadAllEntities(const nlohmann::json& jEntityArray, EntityManager* entityManager);
	bool ReadComponentsOfEntity(const nlohmann::json& jComponentArray, Entity entity, EntityManager* entityManager);

	bool CreateCamera(const nlohmann::json& j, Entity entity, EntityManager* entityManager);
	bool CreateSprite(const nlohmann::json& j, Entity entity, EntityManager* entityManager);
	bool CreateTransform(const nlohmann::json& j, Entity entity, EntityManager* entityManager);

	bool ImportSceneFromFile(const std::string& filePath, bool setToActive)
	{
		std::ifstream inFile(filePath);
		if (inFile.bad())
		{
			return false;
		}

		nlohmann::json jFile = nlohmann::json::parse(inFile);

		std::string name;
		if (!JSON::Read(name, jFile, "name")) {}

		glm::vec3 clearColour;
		if (!JSON::ReadVec3(clearColour, jFile, "clearColour")) {}

		SceneManager* sceneManager = g_app->m_sceneManager;

		sceneManager->CreateScene(name, clearColour);
		Scene* scene = sceneManager->GetScene(name);

		if (!jFile.contains("entities"))
		{
			return false;
		}

		if (!ReadAllEntities(jFile["entities"], scene->m_entityManager))
		{
			return false;
		}

		sceneManager->SetActiveScene(name);

		return true;
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

		if (!j.contains("viewport"))
			return false;

		nlohmann::json jViewport = j["viewport"];

		float viewportWidth;
		if (!JSON::Read(viewportWidth, jViewport, "width"))
		{
			success = false;
		}

		float viewportHeight;
		if (!JSON::Read(viewportHeight, jViewport, "height"))
		{
			success = false;
		}

		float near, far;
		if (!JSON::Read(near, j, "near") || !JSON::Read(far, j, "far")) // If near and far are not specified, use defaults
		{
			Camera* camera = entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight);
		}
		else
		{
			Camera* camera = entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight, near, far);
		}

		bool isActive = false;
		if (!JSON::Read(isActive, j, "isActive")) {}

		if (isActive)
		{
			g_app->SetActiveCamera(entity);
		}

		return success;
	}

	bool CreateSprite(const nlohmann::json& j, Entity entity, EntityManager* entityManager)
	{
		bool success = true;

		glm::vec3 colour;
		if (!JSON::ReadVec3(colour, j, "colour")) {}

		Shader* shader;
		if (j.contains("shader"))
		{
			nlohmann::json jShader = j["shader"];

			std::string name;
			if (!JSON::Read(name, jShader, "name")) {}

			std::string vertexPath, fragmentPath;
			if (!JSON::Read(vertexPath, jShader, "vertexFilePath") || !JSON::Read(fragmentPath, jShader, "fragmentFilePath"))
			{
				success = false; // Cannot render a sprite without vertex & fragment shader
			}

			std::string geometryPath;
			if (!JSON::Read(geometryPath, jShader, "geometryFilePath")) {}

			shader = ShaderFactory::CreatePipelineShader(name, vertexPath, fragmentPath, geometryPath);
		}
		else
		{
			return false;
		}

		if (!j.contains("texture"))
		{
			return false;
		}

		nlohmann::json jTexture = j["texture"];

		std::string textureName;
		if (!JSON::Read(textureName, jTexture, "name")) {}

		std::string texturePath;
		if (!JSON::Read(texturePath, jTexture, "filePath"))
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
		if (!JSON::ReadVec2(pos, j, "pos")) {}

		glm::vec2 size;
		if (!JSON::ReadVec2(size, j, "size")) {}

		float rotate;
		if (!JSON::Read(rotate, j, "rotate")) {}

		Transform* component = entityManager->AddComponent<Transform>(entity, pos, size, rotate);

		return success;
	}
}