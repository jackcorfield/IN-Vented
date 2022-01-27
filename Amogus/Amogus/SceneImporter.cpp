#include "SceneImporter.h"

#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

#include "JSONHelpers.h"
#include "ShaderFactory.h"
#include "source.h"

#include "Camera.h"
#include "Physics.h"
#include "PlayerMovement.h"
#include "Sprite.h"
#include "Tile.h"
#include "TileMap.h"
#include "Transform.h"

#include "Framebuffer.h"

extern Application* g_app;

namespace SceneImporter
{
	EntityManager* g_entityManager; // Store once to avoid retrieving it over and over

	bool ReadAllEntities(const nlohmann::json& jEntityArray);
	bool ReadComponentsOfEntity(const nlohmann::json& jComponentArray, Entity entity, std::vector<Entity>& allTiles, Entity& tileMapEntity);

	/// Add a prototype here for new components (and define it below with the others) ///
	bool CreateCamera(const nlohmann::json& j, Entity entity);
	bool CreatePhysics(const nlohmann::json& j, Entity entity);
	bool CreatePlayerMovement(const nlohmann::json& j, Entity entity);
	bool CreateSprite(const nlohmann::json& j, Entity entity);
	bool CreateTile(const nlohmann::json& j, Entity entity, std::vector<Entity>& allTiles);
	bool CreateTileMap(const nlohmann::json& j, Entity entity, const std::vector<Entity>& tiles);
	bool CreateTransform(const nlohmann::json& j, Entity entity);

	bool ImportSceneFromFile(const std::string& filePath, bool setToActive)
	{
		std::ifstream inFile(filePath);
		if (!inFile.is_open() || !inFile.good())
		{
			return false;
		}

		std::stringstream data;
		data << inFile.rdbuf();

		nlohmann::json jFile;
		try
		{
			jFile = nlohmann::json::parse(data);
		}
		catch (nlohmann::json::parse_error& e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}

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

		g_entityManager = scene->m_entityManager;
		if (!ReadAllEntities(jFile["entities"]))
		{
			return false;
		}

		sceneManager->SetActiveScene(name);

		return true;
	}

	bool ReadAllEntities(const nlohmann::json& jEntityArray)
	{
		bool success = true;

		// Stores Entities that have Tiles (to be given to TileMap later)
		std::vector<Entity> allTiles;
		Entity tileMapEntity = 0; // Stores Entity that owns TileMap, if there is one

		for (int i = 0; i < jEntityArray.size(); i++)
		{
			Entity entity = g_entityManager->CreateEntity();

			if (!ReadComponentsOfEntity(jEntityArray[i], entity, allTiles, tileMapEntity))
			{
				success = false;
			}
		}

		// Create TileMap if an Entity owns one
		// We have to create the TileMap last to allow all Tiles to be created beforehand
		if (tileMapEntity != 0)
		{
			nlohmann::json jEntity = jEntityArray[tileMapEntity - 1];
			nlohmann::json jComponent;

			// Find the TileMap json object
			for (int i = 0; i < jEntity.size(); i++)
			{
				std::string type = jEntity[i]["type"];
				if (jEntity[i]["type"] == "tileMap")
				{
					jComponent = jEntity[i];
					break;
				}
			}

			// If TileMap json object found, create the TileMap
			if (jComponent.is_object() && !CreateTileMap(jComponent, tileMapEntity, allTiles)) { success = false; }
		}

		return success;
	}

	bool ReadComponentsOfEntity(const nlohmann::json& jComponentArray, Entity entity, std::vector<Entity>& allTiles, Entity& tileMapEntity)
	{
		bool success = true;

		for (int i = 0; i < jComponentArray.size(); i++)
		{
			nlohmann::json jComponent = jComponentArray[i];

			std::string componentType;
			if (!JSON::Read(componentType, jComponent, "type")) { success = false; continue; } // Cannot read component if type cannot be determined

			/// Add a new else if block here for new components ///
			if (componentType == "camera")
			{
				if (!CreateCamera(jComponent, entity)) { success = false; }
			}
			else if (componentType == "physics")
			{
				if (!CreatePhysics(jComponent, entity)) { success = false; }
			}
			else if (componentType == "playerMovement")
			{
				if (!CreatePlayerMovement(jComponent, entity)) { success = false; }
			}
			else if (componentType == "sprite")
			{
				if (!CreateSprite(jComponent, entity)) { success = false; }
			}
			else if (componentType == "tile")
			{
				if (!CreateTile(jComponent, entity, allTiles)) { success = false; }
			}
			else if (componentType == "tileMap")
			{
				if (tileMapEntity == 0)
				{
					tileMapEntity = entity;
				}
			}
			else if (componentType == "transform")
			{
				if (!CreateTransform(jComponent, entity)) { success = false; }
			}
		}

		return success;
	}

	bool CreateCamera(const nlohmann::json& j, Entity entity)
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
			Camera* camera = g_entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight, -1.0f, 1.0f, new Framebuffer());
		}
		else
		{
			Camera* camera = g_entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight, near, far, new Framebuffer());
		}

		bool isActive = false;
		if (!JSON::Read(isActive, j, "isActive")) {}

		if (isActive)
		{
			g_app->SetActiveCamera(entity);
		}

		return success;
	}

	bool CreatePhysics(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		float mass = 0.0f;
		if (!JSON::Read(mass, j, "mass")) { success = false; }

		Physics* component = g_entityManager->AddComponent<Physics>(entity, mass);

		return success;
	}

	bool CreatePlayerMovement(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		float speed = 0.0f;
		if (!JSON::Read(speed, j, "speed")) { success = false; }

		PlayerMovement* component = g_entityManager->AddComponent<PlayerMovement>(entity, speed);

		return success;
	}

	bool CreateSprite(const nlohmann::json& j, Entity entity)
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

		Sprite* component = g_entityManager->AddComponent<Sprite>(entity, texture, colour, shader);

		return success;
	}

	bool CreateTile(const nlohmann::json& j, Entity entity, std::vector<Entity>& allTiles)
	{
		bool success = true;

		int tileType = (int)TileObject::NONE;
		if (!JSON::Read(tileType, j, "tileType")) { success = false; }

		Tile* component = g_entityManager->AddComponent<Tile>(entity, (TileObject)tileType);
		if (component)
		{
			allTiles.emplace_back(entity);
		}

		return success;
	}

	bool CreateTileMap(const nlohmann::json& j, Entity entity, const std::vector<Entity>& tiles)
	{
		bool success = true;

		glm::vec2 tileSize;
		if (!JSON::ReadVec2(tileSize, j, "tileSize")) { success = false; }

		glm::vec2 mapSize;
		if (!JSON::ReadVec2(mapSize, j, "mapSize")) { success = false; }

		TileMap* component = g_entityManager->AddComponent<TileMap>(entity, tileSize, mapSize, tiles);

		return success;
	}

	bool CreateTransform(const nlohmann::json& j, Entity entity)
	{
		bool success = true;
		
		glm::vec2 pos;
		if (!JSON::ReadVec2(pos, j, "pos")) {}

		glm::vec2 size;
		if (!JSON::ReadVec2(size, j, "size")) {}

		float rotate;
		if (!JSON::Read(rotate, j, "rotate")) {}

		Transform* component = g_entityManager->AddComponent<Transform>(entity, pos, size, rotate);

		return success;
	}
}