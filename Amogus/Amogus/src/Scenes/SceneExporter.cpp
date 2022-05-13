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
		std::string fullFilePath = "Data/Scenes/" + filePath + ".json";

		g_app->m_debugger->Log("Began scene export to: " + fullFilePath, LL_DEBUG);

		std::ofstream outFile(fullFilePath);
		if (!outFile.is_open() || outFile.bad())
		{
			g_app->m_debugger->Log("Failed scene export: failed to open file for writing!", LL_ERROR);
			return false;
		}

		nlohmann::json jFile;

		Scene* scene = g_app->m_sceneManager->GetActiveScene();
		std::string name = g_app->m_sceneManager->GetActiveSceneName();
		if (!JSON::Write(name, jFile["name"]))
		{
			g_app->m_debugger->Log("Failed scene export: failed to export scene name!", LL_ERROR);
			return false; // Need a name for scene map key
		}

		if (!JSON::WriteVec3(scene->m_sceneColour, jFile["clearColour"])) {}

		g_entityManager = scene->m_entityManager;
		if (!WriteAllComponents(jFile["entities"], scene))
		{
			g_app->m_debugger->Log("Failed scene export: failed to export entities and components!", LL_ERROR);
			return false;
		}

		outFile << std::setw(4) << jFile << std::endl; // Write the exported data to file. std::setw(4) modifies the width of the stream to make the resulting file readable
		outFile.close();

		g_app->m_debugger->Log("Completed scene export to: " + fullFilePath, LL_DEBUG);

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

		if (!success)
		{
			g_app->m_debugger->Log("Failed scene export: failed to export components of type '" + componentJSONName + "'!", LL_ERROR);
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
			if (!JSON::Write(shaderName, jShader["name"]))
			{
				g_app->m_debugger->Log("Exporting AnimatedSprite: failed to write shader name.", LL_WARNING);
			}

			const std::string vertexPath = shader->m_vertexPath;
			const std::string fragmentPath = shader->m_fragmentPath;
			if (!JSON::Write(vertexPath, jShader["vertexFilePath"]) || !JSON::Write(fragmentPath, jShader["fragmentFilePath"]))
			{
				g_app->m_debugger->Log("Failed to export AnimatedSprite: failed to write shader file paths!", LL_ERROR);
				return false; // Cannot render a sprite without vertex & fragment shader
			}

			const std::string geometryPath = shader->m_geometryPath;
			if (!geometryPath.empty())
			{
				if (!JSON::Write(geometryPath, jShader["geometryFilePath"])) {} // Does not need a log as this is an optional field
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
			if (jAnimatedSprite["animations"].is_null())
			{
				jAnimatedSprite["animations"] = nlohmann::json::array();
			}

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

				jAnimatedSprite["animations"].push_back(jAnimationData);
			}
		}

		return success;
	}

	bool WriteAudio(nlohmann::json& jAudio, Audio* audio)
	{
		bool success = true;

		const std::string filePath = audio->m_filePath;
		if (!JSON::Write(filePath, jAudio["filePath"]))
		{
			g_app->m_debugger->Log("Failed to export Audio: failed to write audio file path!", LL_ERROR);
			success = false;
		}

		const FMOD::ChannelGroup* channelGroup = g_app->m_audioManager->GetGroup(audio->m_channel);
		std::string channelGroupString;
		if (channelGroup == g_app->m_audioManager->m_bgm) // Pointer comparison is the simplest solution without adding some kind of identifier in Audio
		{
			channelGroupString = "bgm";
		}
		else if (channelGroup == g_app->m_audioManager->m_sfx)
		{
			channelGroupString = "sfx";
		}
		else
		{
			g_app->m_debugger->Log("Exporting Audio: defaulted channel group to 'sfx'.", LL_WARNING);
			channelGroupString = "sfx";
		}

		if (!JSON::Write(channelGroupString, jAudio["channelGroup"]))
		{
			g_app->m_debugger->Log("Failed to export Audio: failed to write channel group!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WriteBoxCollider(nlohmann::json& jBoxCollider, BoxCollider* boxCollider)
	{
		bool success = true;

		const glm::vec2 size = boxCollider->m_size;
		if (!JSON::WriteVec2(size, jBoxCollider["size"]))
		{
			g_app->m_debugger->Log("Failed to export BoxCollider: failed to write size!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WriteCamera(nlohmann::json& jCamera, Camera* camera)
	{
		bool success = true;

		const float viewportWidth = camera->m_viewportWidth;
		if (!JSON::Write(viewportWidth, jCamera["viewport"]["width"]))
		{
			g_app->m_debugger->Log("Failed to export Camera: failed to write viewport width!", LL_ERROR);
			success = false;
		}

		const float viewportHeight = camera->m_viewportHeight;
		if (!JSON::Write(viewportHeight, jCamera["viewport"]["height"]))
		{
			g_app->m_debugger->Log("Failed to export Camera: failed to write viewport height!", LL_ERROR);
			success = false;
		}

		const float near = camera->m_near;
		if (!JSON::Write(near, jCamera["near"]))
		{
			g_app->m_debugger->Log("Failed to export Camera: failed to write near clip!", LL_ERROR);
			success = false;
		}

		const float far = camera->m_far;
		if (!JSON::Write(far, jCamera["far"]))
		{
			g_app->m_debugger->Log("Failed to export Camera: failed to write far clip!", LL_ERROR);
			success = false;
		}

		const bool isActive = camera->m_isActive;
		if (!JSON::Write(isActive, jCamera["isActive"]))
		{
			g_app->m_debugger->Log("Failed to export Camera: failed to write active value!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WriteCircleCollider(nlohmann::json& jCircleCollider, CircleCollider* circleCollider)
	{
		bool success = true;

		const glm::vec2 centre = circleCollider->m_centre;
		if (!JSON::WriteVec2(centre, jCircleCollider["centre"]))
		{
			g_app->m_debugger->Log("Failed to export CircleCollider: failed to write centre!", LL_ERROR);
			success = false;
		}

		const float radius = circleCollider->m_radius;
		if (!JSON::Write(radius, jCircleCollider["radius"]))
		{
			g_app->m_debugger->Log("Failed to export CircleCollider: failed to write radius!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool SceneExporter::WriteEntityName(nlohmann::json& jEntityName, EntityName* entityName)
	{
		bool success = true;

		const std::string name = entityName->m_name;
		if (!JSON::Write(name, jEntityName["name"]))
		{
			g_app->m_debugger->Log("Failed to export EntityName: failed to write name!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WritePhysics(nlohmann::json& jPhysics, Physics* physics)
	{
		bool success = true;

		const float mass = physics->m_mass;
		if (!JSON::Write(mass, jPhysics["mass"]))
		{
			g_app->m_debugger->Log("Failed to export Physics: failed to write mass!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WritePlayerMovement(nlohmann::json& jPlayerMovement, PlayerMovement* playerMovement)
	{
		bool success = true;

		const float speed = playerMovement->m_speed;
		if (!JSON::Write(speed, jPlayerMovement["speed"]))
		{
			g_app->m_debugger->Log("Failed to export PlayerMovement: failed to write speed!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WriteScriptComponent(nlohmann::json& jScriptComponent, ScriptComponent* scriptComponent)
	{
		bool success = true;

		return success;
	}

	bool WriteSprite(nlohmann::json& jSprite, Sprite* sprite)
	{
		bool success = true;

		const glm::vec3 colour = sprite->GetColour();
		if (!JSON::WriteVec3(colour, jSprite["colour"]))
		{
			g_app->m_debugger->Log("Failed to export Sprite: failed to write tint colour!", LL_ERROR);
			success = false;
		}

		// Write Shader data
		{
			nlohmann::json jShader;
			const Shader* shader = sprite->GetShader();

			const std::string shaderName = shader->m_name;
			if (!JSON::Write(shaderName, jShader["name"]))
			{
				g_app->m_debugger->Log("Exporting Sprite: failed to write shader name.", LL_WARNING);
			}

			const std::string vertexPath = shader->m_vertexPath;
			const std::string fragmentPath = shader->m_fragmentPath;
			if (!JSON::Write(vertexPath, jShader["vertexFilePath"]) || !JSON::Write(fragmentPath, jShader["fragmentFilePath"]))
			{
				g_app->m_debugger->Log("Failed to export Sprite: failed to write shader file paths!", LL_ERROR);
				return false; // Cannot render a sprite without vertex & fragment shader
			}

			const std::string geometryPath = shader->m_geometryPath;
			if (!geometryPath.empty())
			{
				if (!JSON::Write(geometryPath, jShader["geometryFilePath"])) {} // Does not need a log as this an optional field
			}

			jSprite["shader"] = jShader;
		}

		// Write Texture2D data
		{
			nlohmann::json jTexture;
			const Texture2D texture = sprite->GetTexture();

			const std::string textureName = sprite->GetTexture().m_name;
			if (!JSON::Write(textureName, jTexture["name"]))
			{
				g_app->m_debugger->Log("Exporting Sprite: failed to write texture name.", LL_WARNING);
			}

			const std::string texturePath = sprite->GetTexture().m_filePath;
			if (!JSON::Write(texturePath, jTexture["filePath"]))
			{
				g_app->m_debugger->Log("Failed to export Sprite: failed to write texture file path!", LL_ERROR);
				return false; // Cannot render a sprite without a texture
			}

			jSprite["texture"] = jTexture;
		}

		return success;
	}

	bool WriteTile(nlohmann::json& jTile, Tile* tile)
	{
		bool success = true;

		const TileObject tileType = tile->m_object;
		if (!JSON::Write((int)tileType, jTile["tileType"]))
		{
			g_app->m_debugger->Log("Failed to export Tile: failed to write tile type!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WriteTileMap(nlohmann::json& jTileMap, TileMap* tileMap)
	{
		bool success = true;

		const glm::vec2 tileSize = tileMap->m_tileSize;
		if (!JSON::WriteVec2(tileSize, jTileMap["tileSize"]))
		{
			g_app->m_debugger->Log("Failed to export TileMap: failed to write tile size!", LL_ERROR);
			success = false;
		}

		const glm::vec2 mapSize = tileMap->m_mapSize;
		if (!JSON::WriteVec2(mapSize, jTileMap["mapSize"]))
		{
			g_app->m_debugger->Log("Failed to export TileMap: failed to write map size!", LL_ERROR);
			success = false;
		}

		return success;
	}

	bool WriteTransform(nlohmann::json& jTransform, Transform* transform)
	{
		bool success = true;

		const glm::vec2 pos = transform->m_position;
		if (!JSON::WriteVec2(pos, jTransform["pos"]))
		{
			g_app->m_debugger->Log("Failed to export Transform: failed to write position!", LL_ERROR);
			success = false;
		}

		const glm::vec2 size = transform->m_size;
		if (!JSON::WriteVec2(size, jTransform["size"]))
		{
			g_app->m_debugger->Log("Failed to export Transform: failed to write size!", LL_ERROR);
			success = false;
		}

		const float rotate = transform->m_rotate;
		if (!JSON::Write(rotate, jTransform["rotate"]))
		{
			g_app->m_debugger->Log("Failed to export Transform: failed to write rotation!", LL_ERROR);
			success = false;
		}

		return success;
	}
}