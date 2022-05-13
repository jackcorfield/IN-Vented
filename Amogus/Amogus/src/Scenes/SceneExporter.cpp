#include "SceneExporter.h"

#include <fstream>
#include <iomanip>
#include <nlohmann/include/nlohmann/json.hpp>

#include <Handlers/JSONHelpers.h>
#include <Core/source.h>

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

namespace SceneExporter
{
	EntityManager* g_entityManager; // Store once to avoid retrieving it over and over

	bool WriteAllComponents(nlohmann::json& jEntityArray, Scene* scene);

	template <typename T>
	bool WriteComponentsOfType(nlohmann::json& jEntityArray, const std::string& componentJSONName, std::function<bool(nlohmann::json&, T*)> function);

	/// Add a prototype here for new components (and define it below with the others) ///
	bool WriteAnimatedSprite(nlohmann::json& jAnimatedSprite, AnimatedSprite* animatedSprite);
	bool WriteAudio(nlohmann::json& jAudio, Audio* audio);
	bool WriteBoxCollider(nlohmann::json& jBoxCollider, BoxCollider* boxCollider);
	bool WriteCamera(nlohmann::json& jCamera, Camera* camera);
	bool WriteCircleCollider(nlohmann::json& jCircleCollider, CircleCollider* circleCollider);
	bool WriteEntityName(nlohmann::json& jEntityName, EntityName* entityName);
	bool WritePhysics(nlohmann::json& jPhysics, Physics* physics);
	bool WritePlayerMovement(nlohmann::json& jPlayerMovement, PlayerMovement* playerMovement);
	bool WriteScriptComponent(nlohmann::json& jScriptComponent, ScriptComponent* scriptComponent);
	bool WriteSprite(nlohmann::json& jSprite, Sprite* sprite);
	bool WriteTile(nlohmann::json& jTile, Tile* tile);
	bool WriteTileMap(nlohmann::json& jTileMap, TileMap* tileMap);
	bool WriteTransform(nlohmann::json& jTransform, Transform* transform);

	bool ExportActiveSceneToFile(const std::string& filePath)
	{
		Scene* scene = g_app->m_sceneManager->GetActiveScene();

		std::ofstream outFile("Data/Scenes/" + filePath + ".json");
		if (!outFile.is_open() || outFile.bad())
		{
			return false;
		}

		nlohmann::json jFile;

		std::string name = g_app->m_sceneManager->GetActiveSceneName();
		if (!JSON::Write(name, jFile["name"]))
		{
			return false; // Need a name for scene map key
		}

		if (!JSON::WriteVec3(scene->m_sceneColour, jFile["clearColour"])) {}

		g_entityManager = scene->m_entityManager;
		if (!WriteAllComponents(jFile["entities"], scene))
		{
			return false;
		}

		outFile << std::setw(4) << jFile << std::endl; // Write the exported data to file. std::setw(4) modifies the width of the stream to make the resulting file readable
		outFile.close();

		return true;
	}

	bool WriteAllComponents(nlohmann::json& jEntityArray, Scene* scene)
	{
		bool success = true;

		/// Add a new if statement here for new components ///
		if (!WriteComponentsOfType<AnimatedSprite>(jEntityArray, "animatedSprite", WriteAnimatedSprite)) { success = false; }
		if (!WriteComponentsOfType<Audio>(jEntityArray, "audio", WriteAudio)) { success = false; }
		if (!WriteComponentsOfType<BoxCollider>(jEntityArray, "boxCollider", WriteBoxCollider)) { success = false; }
		if (!WriteComponentsOfType<Camera>(jEntityArray, "camera", WriteCamera)) { success = false; }
		if (!WriteComponentsOfType<CircleCollider>(jEntityArray, "circleCollider", WriteCircleCollider)) { success = false; }
		if (!WriteComponentsOfType<EntityName>(jEntityArray, "entityName", WriteEntityName)) { success = false; }
		if (!WriteComponentsOfType<Physics>(jEntityArray, "physics", WritePhysics)) { success = false; }
		if (!WriteComponentsOfType<PlayerMovement>(jEntityArray, "playerMovement", WritePlayerMovement)) { success = false; }
		if (!WriteComponentsOfType<ScriptComponent>(jEntityArray, "scriptComponent", WriteScriptComponent)) { success = false; }
		if (!WriteComponentsOfType<Sprite>(jEntityArray, "sprite", WriteSprite)) { success = false; }
		if (!WriteComponentsOfType<Tile>(jEntityArray, "tile", WriteTile)) { success = false; }
		if (!WriteComponentsOfType<TileMap>(jEntityArray, "tileMap", WriteTileMap)) { success = false; }
		if (!WriteComponentsOfType<Transform>(jEntityArray, "transform", WriteTransform)) { success = false; }

		return success;
	}

	template <typename T>
	bool WriteComponentsOfType(nlohmann::json& jEntityArray, const std::string& componentJSONName, std::function<bool(nlohmann::json&, T*)> function)
	{
		std::vector<T*> allComponents = g_entityManager->GetAllComponentsOfType<T>();

		bool success = true;

		for (int i = 0; i < allComponents.size(); i++)
		{
			Entity entity = g_entityManager->GetEntityFromComponent<T>(allComponents[i]);

			nlohmann::json jComponent = nlohmann::json::object();
			jComponent["type"] = componentJSONName;

			if (!function(jComponent, allComponents[i]))
			{
				success = false;
			}

			if (jEntityArray[entity - 1].is_null())
			{
				jEntityArray[entity - 1] = nlohmann::json::array();
			}
			jEntityArray[entity - 1].push_back(jComponent);
		}

		return success;
	}

	bool WriteAnimatedSprite(nlohmann::json& jAnimatedSprite, AnimatedSprite* animatedSprite)
	{
		bool success = true;

		const glm::vec3 colour = animatedSprite->GetColour();
		if (!JSON::WriteVec3(colour, jAnimatedSprite["colour"]))
		{
			success = false;
		}

		const glm::vec2 frameSize = animatedSprite->getFrameSize();
		if (!JSON::WriteVec2(frameSize, jAnimatedSprite["frameSize"]))
		{
			success = false;
		}

		// Write Shader data
		{
			nlohmann::json jShader;
			const Shader* shader = animatedSprite->GetShader();

			const std::string shaderName = shader->m_name;
			if (!JSON::Write(shaderName, jShader["name"])) {}

			const std::string vertexPath = shader->m_vertexPath;
			const std::string fragmentPath = shader->m_fragmentPath;
			if (!JSON::Write(vertexPath, jShader["vertexFilePath"]) || !JSON::Write(fragmentPath, jShader["fragmentFilePath"]))
			{
				return false; // Cannot render a sprite without vertex & fragment shader
			}

			const std::string geometryPath = shader->m_geometryPath;
			if (!geometryPath.empty())
			{
				if (!JSON::Write(geometryPath, jShader["geometryFilePath"])) {}
			}

			jAnimatedSprite["shader"] = jShader;
		}

		// Write Texture2D data
		{
			nlohmann::json jTexture;
			const Texture2D texture = animatedSprite->GetTexture();

			const std::string textureName = animatedSprite->GetTexture().m_name;
			if (!JSON::Write(textureName, jTexture["name"])) {}

			const std::string texturePath = animatedSprite->GetTexture().m_filePath;
			if (!JSON::Write(texturePath, jTexture["filePath"])) { return false; }// Cannot render a sprite without a texture

			jAnimatedSprite["texture"] = jTexture;
		}

		// Write Animation data
		{
			nlohmann::json jAnimation;
			const std::map<std::string, Animation> animations = animatedSprite->getAnimations();
			
			for (auto& animation : animations)
			{
				nlohmann::json jAnimationData;
				const Animation& animationData = animation.second;
				
				const std::string animationName = animation.first;
				if (!JSON::Write(animationName, jAnimationData["name"])) { success = false; }
				
				const float frameTime = animationData.frameTime;
				if (!JSON::Write(frameTime, jAnimationData["frameTime"])) { success = false; }

				const std::vector<unsigned int> frames = animationData.frames;
				if (!JSON::Write(frames, jAnimationData["frames"])) { success = false; }

				jAnimation["animations"].push_back(jAnimationData);
			}
		}

		return success;
	}

	bool WriteAudio(nlohmann::json& jAudio, Audio* audio)
	{
		bool success = true;

		const std::string filePath = audio->m_filePath;
		if (!JSON::Write(filePath, jAudio["filePath"])) { success = false; }

		const FMOD::ChannelGroup* channelGroup = g_app->m_audioManager->GetGroup(audio->m_channel);
		std::string channelGroupString;
		if (channelGroup == g_app->m_audioManager->m_bgm) // Gross but the simplest solution without adding some kind of identifier in Audio
		{
			channelGroupString = "bgm";
		}
		else if (channelGroup == g_app->m_audioManager->m_sfx)
		{
			channelGroupString = "sfx";
		}
		else
		{
			channelGroupString = "sfx";
		}

		if (!JSON::Write(channelGroupString, jAudio["channelGroup"])) { success = false; }

		return success;
	}

	bool WriteBoxCollider(nlohmann::json& jBoxCollider, BoxCollider* boxCollider)
	{
		bool success = true;

		const glm::vec2 size = boxCollider->m_size;
		if (!JSON::WriteVec2(size, jBoxCollider["size"])) { success = false; }

		return success;
	}

	bool WriteCamera(nlohmann::json& jCamera, Camera* camera)
	{
		bool success = true;

		const float viewportWidth = camera->m_viewportWidth;
		if (!JSON::Write(viewportWidth, jCamera["viewport"]["width"])) { success = false; }

		const float viewportHeight = camera->m_viewportHeight;
		if (!JSON::Write(viewportHeight, jCamera["viewport"]["height"])) { success = false; }

		const float near = camera->m_near;
		if (!JSON::Write(near, jCamera["near"])) { success = false; }

		const float far = camera->m_far;
		if (!JSON::Write(far, jCamera["far"])) { success = false; }

		const bool isActive = camera->m_isActive;
		if (!JSON::Write(isActive, jCamera["isActive"])) { success = false; }

		return success;
	}

	bool WriteCircleCollider(nlohmann::json& jCircleCollider, CircleCollider* circleCollider)
	{
		bool success = true;

		const glm::vec2 centre = circleCollider->m_centre;
		if (!JSON::WriteVec2(centre, jCircleCollider["centre"])) { success = false; }

		const float radius = circleCollider->m_radius;
		if (!JSON::Write(radius, jCircleCollider["radius"])) { success = false; }

		return success;
	}

	bool SceneExporter::WriteEntityName(nlohmann::json& jEntityName, EntityName* entityName)
	{
		bool success = true;

		const std::string name = entityName->m_name;
		if (!JSON::Write(name, jEntityName["name"])) { success = false; }

		return success;
	}

	bool WritePhysics(nlohmann::json& jPhysics, Physics* physics)
	{
		bool success = true;

		const float mass = physics->m_mass;
		if (!JSON::Write(mass, jPhysics["mass"])) { success = false; }

		return success;
	}

	bool WritePlayerMovement(nlohmann::json& jPlayerMovement, PlayerMovement* playerMovement)
	{
		bool success = true;

		const float speed = playerMovement->m_speed;
		if (!JSON::Write(speed, jPlayerMovement["speed"])) { success = false; }

		return success;
	}

	bool WriteScriptComponent(nlohmann::json& jScriptComponent, ScriptComponent* scriptComponent)
	{
		bool success = true;

		//if (!JSON::Write("", jScriptComponent[""])) { success = false; }

		return success;
	}

	bool WriteSprite(nlohmann::json& jSprite, Sprite* sprite)
	{
		bool success = true;

		const glm::vec3 colour = sprite->GetColour();
		if (!JSON::WriteVec3(colour, jSprite["colour"]))
		{
			success = false;
		}

		// Write Shader data
		{
			nlohmann::json jShader;
			const Shader* shader = sprite->GetShader();

			const std::string shaderName = shader->m_name;
			if (!JSON::Write(shaderName, jShader["name"])) {}

			const std::string vertexPath = shader->m_vertexPath;
			const std::string fragmentPath = shader->m_fragmentPath;
			if (!JSON::Write(vertexPath, jShader["vertexFilePath"]) || !JSON::Write(fragmentPath, jShader["fragmentFilePath"]))
			{
				return false; // Cannot render a sprite without vertex & fragment shader
			}

			const std::string geometryPath = shader->m_geometryPath;
			if (!geometryPath.empty())
			{
				if (!JSON::Write(geometryPath, jShader["geometryFilePath"])) {}
			}

			jSprite["shader"] = jShader;
		}

		// Write Texture2D data
		{
			nlohmann::json jTexture;
			const Texture2D texture = sprite->GetTexture();

			const std::string textureName = sprite->GetTexture().m_name;
			if (!JSON::Write(textureName, jTexture["name"])) {}

			const std::string texturePath = sprite->GetTexture().m_filePath;
			if (!JSON::Write(texturePath, jTexture["filePath"])) { return false; }// Cannot render a sprite without a texture

			jSprite["texture"] = jTexture;
		}

		return success;
	}

	bool WriteTile(nlohmann::json& jTile, Tile* tile)
	{
		bool success = true;

		const TileObject tileType = tile->m_object;
		if (!JSON::Write((int)tileType, jTile["tileType"])) { success = false; }

		return success;
	}

	bool WriteTileMap(nlohmann::json& jTileMap, TileMap* tileMap)
	{
		bool success = true;

		const glm::vec2 tileSize = tileMap->m_tileSize;
		if (!JSON::WriteVec2(tileSize, jTileMap["tileSize"])) { success = false; }

		const glm::vec2 mapSize = tileMap->m_mapSize;
		if (!JSON::WriteVec2(mapSize, jTileMap["mapSize"])) { success = false; }



		return success;
	}

	bool WriteTransform(nlohmann::json& jTransform, Transform* transform)
	{
		bool success = true;

		const glm::vec2 pos = transform->m_position;
		if (!JSON::WriteVec2(pos, jTransform["pos"])) { success = false; }

		const glm::vec2 size = transform->m_size;
		if (!JSON::WriteVec2(size, jTransform["size"])) { success = false; }

		const float rotate = transform->m_rotate;
		if (!JSON::Write(rotate, jTransform["rotate"])) { success = false; }

		return success;
	}
}