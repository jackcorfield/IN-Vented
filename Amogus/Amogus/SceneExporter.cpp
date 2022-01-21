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

		//jFile["name"] = scene.m_name; how can i get this?
		//if (!JSON::Write(scene.m_name, jFile["name"]))
		//{
		//	return false; // Need a name for scene map key
		//}

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

		return success;
	}

	bool WriteSprite(nlohmann::json& jSprite, Sprite* sprite)
	{
		bool success = true;

		const glm::vec3 colour = glm::vec3(0.0f);
		if (!JSON::WriteVec3(colour, jSprite["colour"]))
		{
			success = false;
		}

		//const std::string vertexPath = component->GetShader()->GetVertexPath();
		//const std::string fragmentPath = component->GetShader()->GetFragmentPath();
		//if (!JSON::Write(vertexPath, jSprite["vertexFilePath"]) || !JSON::Write(fragmentPath, jSprite["fragmentFilePath"]))
		//{
			//return false; // Cannot render a sprite without vertex & fragment shader
		//}

		//const std::string geometryPath = component->GetShader()->GetGeometryPath();
		//if (!geometryPath.empty())
		//{
		//	if (!JSON::Write(geometryPath, jSprite["geometryFilePath"])) {}
		//}

		// const std::string textureName = component->GetTexture().m_name;
		//if (!JSON::Write(textureName, jSprite["texture"]["name"])) {}

		// const std::string texturePath = component->GetTexture().m_filePath;
		//if (!JSON::Write(texturePath, jSprite["texture"]["filePath"]))
		//{
		//	return false; // Cannot render a sprite without a texture
		//}

		return true;

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