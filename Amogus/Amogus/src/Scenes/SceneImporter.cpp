#include "SceneImporter.h"

#include <fstream>
#include <sstream>
#include <nlohmann/include/nlohmann/json.hpp>

#include <Handlers/JSONHelpers.h>
#include <ResourceFactories/ShaderFactory.h>
#include <Core/source.h>

#include <Resources/Framebuffer.h>

// Component includes
#include <ECS/Components/AnimatedSprite.h>
#include <ECS/Components/Audio.h>
#include <ECS/Components/BoxCollider.h>
#include <ECS/Components/Camera.h>
#include <ECS/Components/CircleCollider.h>
#include <ECS/Components/EntityName.h>
#include <ECS/Components/Physics.h>
#include <ECS/Components/PlayerMovement.h>
#include <ECS/Components/ScriptComponent.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/Tile.h>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/Transform.h>


extern Application* g_app;

namespace SceneImporter
{
	EntityManager* g_entityManager; // Store once to avoid retrieving it over and over

	bool ReadAllEntities(const nlohmann::json& jEntityArray);
	bool ReadComponentsOfEntity(const nlohmann::json& jComponentArray, Entity entity, std::vector<Entity>& allTiles, Entity& tileMapEntity);

	/// Add a prototype here for new components (and define it below with the others) ///
	bool CreateAnimatedSprite(const nlohmann::json& j, Entity entity);
	bool CreateAudio(const nlohmann::json& j, Entity entity);
	bool CreateBoxCollider(const nlohmann::json& j, Entity entity);
	bool CreateCamera(const nlohmann::json& j, Entity entity);
	bool CreateCircleCollider(const nlohmann::json& j, Entity entity);
	bool CreateEntityName(const nlohmann::json& j, Entity entity);
	bool CreatePhysics(const nlohmann::json& j, Entity entity);
	bool CreatePlayerMovement(const nlohmann::json& j, Entity entity);
	bool CreateScriptComponent(const nlohmann::json& j, Entity entity);
	bool CreateSprite(const nlohmann::json& j, Entity entity);
	bool CreateTile(const nlohmann::json& j, Entity entity, std::vector<Entity>& allTiles);
	bool CreateTileMap(const nlohmann::json& j, Entity entity, const std::vector<Entity>& tiles);
	bool CreateTransform(const nlohmann::json& j, Entity entity);

	// Helpers
	bool ReadShader(const nlohmann::json& jShader, Shader** readTo);
	bool ReadTexture(const nlohmann::json& jTexture, Texture2D& readTo);

	bool ImportSceneFromFile(const std::string& filePath, bool setToActive)
	{
		std::string fullFilePath = "Data/Scenes/" + filePath + ".json";

		g_app->m_debugger->Log("Began scene import from: " + fullFilePath, LL_DEBUG);

		std::ifstream inFile(fullFilePath);
		if (!inFile.is_open() || !inFile.good())
		{
			g_app->m_debugger->Log("Failed scene import: failed to open file for reading!", LL_ERROR);
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
			g_app->m_debugger->Log("Failed scene import: JSON parse error!", LL_ERROR);
			return false;
		}

		std::string name;
		if (!JSON::Read(name, jFile, "name")) {}
		if (g_app->m_sceneManager->SceneExists(name))
		{
			g_app->m_debugger->Log("Failed scene import: scene with same name already loaded!", LL_ERROR);
			return false;
		}

		glm::vec3 clearColour;
		if (!JSON::ReadVec3(clearColour, jFile, "clearColour")) {}

		SceneManager* sceneManager = g_app->m_sceneManager;

		sceneManager->CreateScene(name, clearColour);
		Scene* scene = sceneManager->GetScene(name);
		if (setToActive)
		{
			sceneManager->SetActiveScene(name);
		}

		if (!jFile.contains("entities"))
		{
			g_app->m_debugger->Log("Failed scene import: missing JSON data!", LL_ERROR);
			return false;
		}

		g_entityManager = scene->m_entityManager;
		if (!ReadAllEntities(jFile["entities"]))
		{
			g_app->m_debugger->Log("Failed scene import: failed to import entities and components!", LL_ERROR);
			return false;
		}

		g_app->m_debugger->Log("Completed scene import from: " + fullFilePath, LL_DEBUG);

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
		//if (tileMapEntity != 0)
		//{
		//	nlohmann::json jEntity = jEntityArray[tileMapEntity - 1];
		//	nlohmann::json jComponent;

		//	// Find the TileMap json object
		//	for (int i = 0; i < jEntity.size(); i++)
		//	{
		//		std::string type = jEntity[i]["type"];
		//		if (jEntity[i]["type"] == "tileMap")
		//		{
		//			jComponent = jEntity[i];
		//			break;
		//		}
		//	}

		//	// If TileMap json object found, create the TileMap
		//	if (jComponent.is_object() && !CreateTileMap(jComponent, tileMapEntity, allTiles)) { success = false; }
		//}

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
			if (componentType == "animatedSprite")
			{
				if (!CreateAnimatedSprite(jComponent, entity)) { success = false; }
			}
			else if (componentType == "audio")
			{
				if (!CreateAudio(jComponent, entity)) { success = false; }
			}
			else if (componentType == "boxCollider")
			{
				if (!CreateBoxCollider(jComponent, entity)) { success = false; }
			}
			else if (componentType == "camera")
			{
				if (!CreateCamera(jComponent, entity)) { success = false; }
			}
			else if (componentType == "circleCollider")
			{
				if (!CreateCircleCollider(jComponent, entity)) { success = false; }
			}
			else if (componentType == "entityName")
			{
				if (!CreateEntityName(jComponent, entity)) { success = false; }
			}
			else if (componentType == "physics")
			{
				if (!CreatePhysics(jComponent, entity)) { success = false; }
			}
			else if (componentType == "playerMovement")
			{
				if (!CreatePlayerMovement(jComponent, entity)) { success = false; }
			}
			else if (componentType == "scriptComponent")
			{
				if (!CreateScriptComponent(jComponent, entity)) { success = false; }
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

		if (!success)
		{
			g_app->m_debugger->Log("Failed scene import: failed to import entity " + std::to_string(entity) + "!", LL_ERROR);
		}

		return success;
	}

	bool CreateAnimatedSprite(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		glm::vec3 colour = glm::vec3(1.0f);
		if (!JSON::ReadVec3(colour, j, "colour"))
		{
			g_app->m_debugger->Log("Failed to import AnimatedSprite: failed to read tint colour!", LL_ERROR);
			success = false;
		}

		glm::vec2 frameSize = glm::vec2(1.0f);
		if (!JSON::ReadVec2(frameSize, j, "frameSize")) { success = false; }

		Shader* shader = nullptr;
		if (!j.contains("shader") || !ReadShader(j["shader"], &shader))
		{
			g_app->m_debugger->Log("Failed to import AnimatedSprite: failed to read shader!", LL_ERROR);
			return false;
		}

		Texture2D texture;
		if (!j.contains("texture") || !ReadTexture(j["texture"], texture))
		{
			g_app->m_debugger->Log("Failed to import AnimatedSprite: missing JSON data!", LL_ERROR);
			return false;
		}

		AnimatedSprite* component = g_entityManager->AddComponent<AnimatedSprite>(entity, texture, frameSize, colour, shader);
		
		if (!j.contains("animations"))
		{
			return success; // Return early; not a fail state, just no animations stored
		}

		nlohmann::json animations = j["animations"];
		for (auto& animation : animations)
		{
			std::string animationName = animation["name"];
			float frameTime = animation["frameTime"];
			std::vector<unsigned int> frames = animation["frames"];

			component->createAnimation(animationName, frames, frameTime);
		}

		return success;
	}

	bool CreateAudio(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		std::string filePath;
		if (!JSON::Read(filePath, j, "filePath"))
		{
			g_app->m_debugger->Log("Failed to import Audio: failed to read audio file path!", LL_ERROR);
			success = false;
		}

		std::string channelGroupString;
		if (!JSON::Read(channelGroupString, j, "channelGroup"))
		{
			g_app->m_debugger->Log("Failed to import Audio: failed to read channel group!", LL_ERROR);
			success = false;
		}

		FMOD::ChannelGroup* channelGroup = nullptr;
		if (channelGroupString == "bgm")
		{
			channelGroup = g_app->m_audioManager->m_bgm;
		}
		else if (channelGroupString == "sfx")
		{
			channelGroup = g_app->m_audioManager->m_sfx;
		}
		else
		{
			g_app->m_debugger->Log("Importing Audio: defaulted channel group to 'sfx'.", LL_WARNING);
			channelGroup = g_app->m_audioManager->m_sfx;
		}

		FMOD::System* system = g_app->m_audioManager->m_system;

		Audio* component = g_entityManager->AddComponent<Audio>(entity, filePath.c_str(), system, channelGroup);

		return success;
	}

	bool CreateBoxCollider(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		glm::vec2 size;
		if (!JSON::ReadVec2(size, j, "size"))
		{
			g_app->m_debugger->Log("Failed to import BoxCollider: failed to read size!", LL_ERROR);
			success = false;
		}

		BoxCollider* component = g_entityManager->AddComponent<BoxCollider>(entity, size);
		return success;
	}

	bool CreateCamera(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		if (!j.contains("viewport"))
		{
			g_app->m_debugger->Log("Failed to import Camera: missing JSON data 'viewport'!", LL_ERROR);
			return false;
		}

		nlohmann::json jViewport = j["viewport"];

		float viewportWidth;
		if (!JSON::Read(viewportWidth, jViewport, "width"))
		{
			g_app->m_debugger->Log("Failed to import Camera: failed to read viewport width!", LL_ERROR);
			success = false;
		}

		float viewportHeight;
		if (!JSON::Read(viewportHeight, jViewport, "height"))
		{
			g_app->m_debugger->Log("Failed to import Camera: failed to read viewport height!", LL_ERROR);
			success = false;
		}

		if (!j.contains("internalViewport"))
		{
			g_app->m_debugger->Log("Failed to import Camera: missing JSON data 'internalViewport'!", LL_ERROR);
			return false;
		}

		nlohmann::json jInternalViewport = j["internalViewport"];

		float internalWidth;
		if (!JSON::Read(internalWidth, jInternalViewport, "width"))
		{
			g_app->m_debugger->Log("Failed to import Camera: failed to read internal viewport width!", LL_ERROR);
			success = false;
		}

		float internalHeight;
		if (!JSON::Read(internalHeight, jInternalViewport, "height"))
		{
			g_app->m_debugger->Log("Failed to import Camera: failed to read internal viewport height!", LL_ERROR);
			success = false;
		}

		float near, far;
		if (!JSON::Read(near, j, "near") || !JSON::Read(far, j, "far")) // If near and far are not specified, use defaults
		{
			Camera* camera = g_entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight, -1.0f, 1.0f, new Framebuffer());
			camera->m_internalWidth = internalWidth;
			camera->m_internalHeight = internalHeight;
		}
		else
		{
			Camera* camera = g_entityManager->AddComponent<Camera>(entity, viewportWidth, viewportHeight, near, far, new Framebuffer());
			camera->m_internalWidth = internalWidth;
			camera->m_internalHeight = internalHeight;
		}

		bool isActive = false;
		if (!JSON::Read(isActive, j, "isActive"))
		{
			g_app->m_debugger->Log("Importing Camera: failed to read active value.", LL_WARNING);
		}

		if (isActive)
		{
			g_app->SetActiveCamera(entity);
		}

		return success;
	}

	bool CreateCircleCollider(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		glm::vec2 centre;
		if (!JSON::ReadVec2(centre, j, "centre"))
		{
			g_app->m_debugger->Log("Failed to import CircleCollider: failed to read centre!", LL_ERROR);
			success = false;
		}

		float radius;
		if (!JSON::Read(radius, j, "radius"))
		{
			g_app->m_debugger->Log("Failed to import CircleCollider: failed to read radius!", LL_ERROR);
			success = false;
		}

		CircleCollider* component = g_entityManager->AddComponent<CircleCollider>(entity, radius, centre);

		return success;
	}

	bool CreateEntityName(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		std::string name = "";
		if (!JSON::Read(name, j, "name"))
		{
			g_app->m_debugger->Log("Failed to import EntityName: failed to read name!", LL_ERROR);
			success = false;
		}

		EntityName* component = g_entityManager->AddComponent<EntityName>(entity, name);

		return success;
	}

	bool CreatePhysics(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		float mass = 0.0f;
		if (!JSON::Read(mass, j, "mass"))
		{
			g_app->m_debugger->Log("Failed to import Physics: failed to read mass!", LL_ERROR);
			success = false;
		}

		Physics* component = g_entityManager->AddComponent<Physics>(entity, mass);

		return success;
	}

	bool CreatePlayerMovement(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		float speed = 0.0f;
		if (!JSON::Read(speed, j, "speed"))
		{
			g_app->m_debugger->Log("Failed to import PlayerMovement: failed to read speed!", LL_ERROR);
			success = false;
		}

		PlayerMovement* component = g_entityManager->AddComponent<PlayerMovement>(entity, speed);

		return success;
	}

	bool CreateScriptComponent(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		ScriptComponent* component = g_entityManager->AddComponent<ScriptComponent>(entity, g_entityManager, entity);

		return success;
	}

	bool CreateSprite(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		glm::vec3 colour;
		if (!JSON::ReadVec3(colour, j, "colour"))
		{
			g_app->m_debugger->Log("Importing Sprite: failed to read tint colour.", LL_WARNING);
		}

		Shader* shader = nullptr;
		if (!j.contains("shader") || !ReadShader(j["shader"], &shader))
		{
			g_app->m_debugger->Log("Failed to import Sprite: failed to read shader!", LL_ERROR);
			return false;
		}

		Texture2D texture;
		if (!j.contains("texture") || !ReadTexture(j["texture"], texture))
		{
			g_app->m_debugger->Log("Failed to import Sprite: failed to read texture!", LL_ERROR);
			return false;
		}

		Sprite* component = g_entityManager->AddComponent<Sprite>(entity, texture, colour, shader);

		return success;
	}

	bool CreateTile(const nlohmann::json& j, Entity entity, std::vector<Entity>& allTiles)
	{
		bool success = true;

		int tileType = (int)TileObject::NONE;
		if (!JSON::Read(tileType, j, "tileType"))
		{
			g_app->m_debugger->Log("Failed to import Tile: failed to read tile type!", LL_ERROR);
			success = false;
		}

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
		if (!JSON::ReadVec2(tileSize, j, "tileSize"))
		{
			g_app->m_debugger->Log("Failed to import TileMap: failed to read tile size!", LL_ERROR);
			success = false;
		}

		glm::vec2 mapSize;
		if (!JSON::ReadVec2(mapSize, j, "mapSize"))
		{
			g_app->m_debugger->Log("Failed to import TileMap: failed to read map size!", LL_ERROR);
			success = false;
		}

		TileMap* component = g_entityManager->AddComponent<TileMap>(entity, tileSize, mapSize, tiles);

		return success;
	}

	bool CreateTransform(const nlohmann::json& j, Entity entity)
	{
		bool success = true;

		glm::vec2 pos(0.0f);
		if (!JSON::ReadVec2(pos, j, "pos"))
		{
			g_app->m_debugger->Log("Importing Transform: failed to read position.", LL_WARNING);
		}

		glm::vec2 size(1.0f);
		if (!JSON::ReadVec2(size, j, "size"))
		{
			g_app->m_debugger->Log("Importing Transform: failed to read size.", LL_WARNING);
		}

		float rotate(0.0f);
		if (!JSON::Read(rotate, j, "rotate"))
		{
			g_app->m_debugger->Log("Importing Transform: failed to read rotation.", LL_WARNING);
		}

		float depth(0.0f);
		if (!JSON::Read(depth, j, "depth"))
		{
			g_app->m_debugger->Log("Importing Transform: failed to read depth", LL_WARNING);
		}

		Transform* component = g_entityManager->AddComponent<Transform>(entity, pos, size, rotate, depth);

		return success;
	}

	bool ReadShader(const nlohmann::json& jShader, Shader** readTo)
	{
		bool success = true;

		std::string name;
		if (!JSON::Read(name, jShader, "name"))
		{
			g_app->m_debugger->Log("Importing shader: failed to read shader name.", LL_WARNING);
		}

		std::string vertexPath, fragmentPath;
		if (!JSON::Read(vertexPath, jShader, "vertexFilePath") || !JSON::Read(fragmentPath, jShader, "fragmentFilePath"))
		{
			g_app->m_debugger->Log("Failed to import shader: failed to read shader file paths!", LL_ERROR);
			success = false; // Cannot render a sprite without vertex & fragment shader
		}

		std::string geometryPath;
		if (!JSON::Read(geometryPath, jShader, "geometryFilePath")) {} // Does not need a log as this is an optional field

		*readTo = ShaderFactory::CreatePipelineShader(name, vertexPath, fragmentPath, geometryPath);

		return success;
	}

	bool ReadTexture(const nlohmann::json& jTexture, Texture2D& readTo)
	{
		bool success = true;

		std::string textureName;
		if (!JSON::Read(textureName, jTexture, "name"))
		{
			g_app->m_debugger->Log("Importing texture: failed to read texture name.", LL_WARNING);
		}

		std::string texturePath;
		if (!JSON::Read(texturePath, jTexture, "filePath"))
		{
			g_app->m_debugger->Log("Failed to import texture: failed to read texture file path!", LL_ERROR);
			success = false; // Cannot render a sprite without a texture
		}

		readTo = TextureLoader::CreateTexture2DFromFile(textureName, texturePath);

		return success;
	}
}