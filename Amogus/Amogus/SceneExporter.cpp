#include "SceneExporter.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "JSONHelpers.h"
#include "source.h"

#include "Camera.h"
#include "Sprite.h"
#include "Transform.h"

extern Application* g_app;

namespace SceneExporter
{
	bool WriteAllComponents(nlohmann::json& jEntityArray, Scene* scene);

	template <typename T>
	bool WriteComponentsOfType(nlohmann::json& jEntityArray, EntityManager* entityManager, const std::string& componentJSONName, std::function<bool(nlohmann::json&, T*)> function);

	bool WriteCamera(nlohmann::json& jComponentArray, Camera* camera);
	bool WriteSprite(nlohmann::json& jComponentArray, Sprite* camera);
	bool WriteTransform(nlohmann::json& jComponentArray, Transform* camera);

	bool ExportActiveSceneToFile(const std::string& filePath)
	{
		Scene* scene = g_app->m_sceneManager->GetActiveScene();

		std::ofstream outFile(filePath);
		if (outFile.bad())
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

		if (!WriteAllComponents(jFile["entities"], scene))
		{
			return false;
		}

		outFile << std::setw(4) << jFile << std::endl; // Write the exported data to file. std::setw(4) modifies the width of the stream to make the resulting file readable
		outFile.close();
	}

	bool WriteAllComponents(nlohmann::json& jEntityArray, Scene* scene)
	{
		EntityManager* entityManager = scene->m_entityManager;

		bool success = true;

		if (!WriteComponentsOfType<Camera>(jEntityArray, entityManager, "camera", WriteCamera)) { success = false; }
		if (!WriteComponentsOfType<Sprite>(jEntityArray, entityManager, "sprite", WriteSprite)) { success = false; }
		if (!WriteComponentsOfType<Transform>(jEntityArray, entityManager, "transform", WriteTransform)) { success = false; }

		return success;
	}

	template <typename T>
	bool WriteComponentsOfType(nlohmann::json& jEntityArray, EntityManager* entityManager, const std::string& componentJSONName, std::function<bool(nlohmann::json&, T*)> function)
	{
		std::vector<T*> allComponents = entityManager->GetAllComponentsOfType<T>();

		bool success = true;

		for (int i = 0; i < allComponents.size(); i++)
		{
			Entity entity = entityManager->GetEntityFromComponent<T>(allComponents[i]);
			
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

	bool WriteCamera(nlohmann::json& jCamera, Camera* camera)
	{
		bool success = true;

		const float viewportWidth = camera->m_viewportWidth;
		if (!JSON::Write(viewportWidth, jCamera["viewport"]["width"]))
		{
			success = false;
		}

		const float viewportHeight = camera->m_viewportHeight;
		if (!JSON::Write(viewportHeight, jCamera["viewport"]["height"]))
		{
			success = false;
		}

		const float near = camera->m_near;
		if (!JSON::Write(near, jCamera["near"]))
		{
			success = false;
		}

		const float far = camera->m_far;
		if (!JSON::Write(far, jCamera["far"]))
		{
			success = false;
		}

		const bool isActive = camera->m_isActive;
		if (!JSON::Write(isActive, jCamera["isActive"])) {}

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
			if (!JSON::Write(texturePath, jTexture["filePath"]))
			{
				return false; // Cannot render a sprite without a texture
			}

			jSprite["texture"] = jTexture;
		}

		return success;
	}

	bool WriteTransform(nlohmann::json& jTransform, Transform* transform)
	{
		bool success = true;

		const glm::vec2 pos = transform->m_position;
		if (!JSON::WriteVec2(pos, jTransform["pos"]))
		{
			success = false;
		}

		const glm::vec2 size = transform->m_size;
		if (!JSON::WriteVec2(size, jTransform["size"]))
		{
			success = false;
		}

		const float rotate = transform->m_rotate;
		if (!JSON::Write(rotate, jTransform["rotate"]))
		{
			success = false;
		}

		return success;
	}
}