#include "EntityInspectorGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <string.h>

#include <Amogus.h>

#include "ComponentDialogBoxes/NewAnimatedSpriteGui.h"
#include "ComponentDialogBoxes/NewAudioGui.h"
#include "ComponentDialogBoxes/NewBoxColliderGui.h"
#include "ComponentDialogBoxes/NewCircleColliderGui.h"
#include "ComponentDialogBoxes/NewSpriteGui.h"
#include "ComponentDialogBoxes/NewTileMapGui.h"

#define MAX_INPUT_LENGTH 256

#define DEFAULT_VIEWPORT_WIDTH 1280
#define DEFAULT_VIEWPORT_HEIGHT 720
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 1.0f

void DrawComponentGui(void* component, std::type_index type, Entity entity, EntityManager* entityManager, int i);
void DeleteComponent(void* component, std::type_index type, Entity entity, EntityManager* entityManager);

/// Add prototypes here for new components (and define below with the others) ///
// Inspector gui functions
void CreateAnimatedSpriteGui(AnimatedSprite* animatedSprite, Entity owner);
void CreateAudioGui(Audio* audio, Entity owner);
void CreateBoxColliderGui(BoxCollider* boxCollider);
void CreateCameraGui(Camera* camera);
void CreateCircleColliderGui(CircleCollider* circleCollider);
void CreateEntityNameGui(EntityName* entityName);
void CreatePhysicsGui(Physics* physics);
void CreatePlayerMovementGui(PlayerMovement* playerMovement);
void CreateScriptComponentGui(ScriptComponent* scriptComponent);
void CreateSpriteGui(Sprite* sprite, Entity owner);
void CreateTileGui(Tile* tile);
void CreateTileMapGui(TileMap* tileMap);
void CreateTransformGui(Transform* transform);
void CreateUI_ImageGui(UI_Image* image, Entity owner);

// Helpers (return true if changed)
bool CreateShaderGui(std::string& shaderName, std::string& vertexPath, std::string& fragmentPath, std::string& geometryPath);
bool CreateTextureGui(std::string& textureName, std::string& filePath);

EntityInspectorGui::EntityInspectorGui() :
	m_guiObjects(),
	m_activeEntity(0)
{}

EntityInspectorGui::~EntityInspectorGui()
{
	m_guiObjects.clear();
}

void EntityInspectorGui::Draw()
{
	ImGui::Begin("Inspector");

	if (m_activeEntity != 0)
	{
		DrawInspectorInfo();
	}

	// Draw each dialog box
	for (int i = 0; i < m_guiObjects.size(); i++)
	{
		IGuiObject* guiObject = m_guiObjects[i].get();

		guiObject->CreateGui();

		if (guiObject->close) // If this window is ready to close, delete object
		{
			m_guiObjects.erase(m_guiObjects.begin() + i);
		}
	}

	ImGui::End();
}

void EntityInspectorGui::DrawInspectorInfo()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Handle entity name separately from the other components, as it should be at the top
	if (entityManager->HasComponent<EntityName>(m_activeEntity))
	{
		EntityName* name = entityManager->GetComponent<EntityName>(m_activeEntity);
		CreateEntityNameGui(name);
	}

	// Delete entity button. Could possibly add a confirmation dialog box in future
	if (ImGui::Button("Delete entity"))
	{
		entityManager->DeleteEntity(m_activeEntity);
		m_activeEntity = 0;
		return;
	}

	ImGui::Separator();

	// Iterate through all of the other components and draw gui for each
	std::map<std::type_index, void*> allComponentsOfEntity = entityManager->GetAllComponents(m_activeEntity);
	int i = 0;
	for (auto componentPair : allComponentsOfEntity)
	{
		size_t hashCode = componentPair.first.hash_code(); // For comparing types
		std::type_index typeIndex = componentPair.first;
		void* component = componentPair.second; // To pass into correct function

		DrawComponentGui(component, typeIndex, m_activeEntity, entityManager, i);

		ImGui::Separator();

		i++;
	}

	CreateAddComponentGui();
}

void EntityInspectorGui::SetActiveEntity(Entity entity)
{
	m_activeEntity = entity;
}

void DrawComponentGui(void* component, std::type_index type, Entity entity, EntityManager* entityManager, int i)
{
	if (!component)
	{
		return;
	}

	if (type == typeid(AnimatedSprite)) { CreateAnimatedSpriteGui(reinterpret_cast<AnimatedSprite*>(component), entity); }
	else if (type == typeid(Audio)) { CreateAudioGui(reinterpret_cast<Audio*>(component), entity); }
	else if (type == typeid(BoxCollider)) { CreateBoxColliderGui(reinterpret_cast<BoxCollider*>(component)); }
	else if (type == typeid(Camera)) { CreateCameraGui(reinterpret_cast<Camera*>(component)); }
	else if (type == typeid(CircleCollider)) { CreateCircleColliderGui(reinterpret_cast<CircleCollider*>(component)); }
	else if (type == typeid(EntityName)) { return; } // Name has already been handled, so we can skip it here
	else if (type == typeid(Physics)) { CreatePhysicsGui(reinterpret_cast<Physics*>(component)); }
	else if (type == typeid(PlayerMovement)) { CreatePlayerMovementGui(reinterpret_cast<PlayerMovement*>(component)); }
	else if (type == typeid(ScriptComponent)) { CreateScriptComponentGui(reinterpret_cast<ScriptComponent*>(component)); }
	else if (type == typeid(Sprite)) { CreateSpriteGui(reinterpret_cast<Sprite*>(component), entity); }
	else if (type == typeid(Tile)) { CreateTileGui(reinterpret_cast<Tile*>(component)); }
	else if (type == typeid(TileMap)) { CreateTileMapGui(reinterpret_cast<TileMap*>(component)); }
	else if (type == typeid(Transform)) { CreateTransformGui(reinterpret_cast<Transform*>(component)); }
	else if (type == typeid(UI_Image)) { CreateUI_ImageGui(reinterpret_cast<UI_Image*>(component), entity); }
	else
	{
		std::cout << "Error: Invalid component type!" << std::endl;
	}

	std::string buttonName = "Delete##" + std::to_string(i);
	if (ImGui::Button(buttonName.c_str()))
	{
		DeleteComponent(component, type, entity, entityManager);
	}
}

void DeleteComponent(void* component, std::type_index type, Entity entity, EntityManager* entityManager)
{
	if (type == typeid(AnimatedSprite)) { entityManager->RemoveComponent<AnimatedSprite>(entity); }
	else if (type == typeid(Audio)) { entityManager->RemoveComponent<Audio>(entity); }
	else if (type == typeid(BoxCollider)) { entityManager->RemoveComponent<BoxCollider>(entity); }
	else if (type == typeid(Camera)) { entityManager->RemoveComponent<Camera>(entity); }
	else if (type == typeid(CircleCollider)) { entityManager->RemoveComponent<CircleCollider>(entity); }
	else if (type == typeid(EntityName)) { entityManager->RemoveComponent<EntityName>(entity); }
	else if (type == typeid(Physics)) { entityManager->RemoveComponent<Physics>(entity); }
	else if (type == typeid(PlayerMovement)) { entityManager->RemoveComponent<PlayerMovement>(entity); }
	else if (type == typeid(ScriptComponent)) { entityManager->RemoveComponent<ScriptComponent>(entity); }
	else if (type == typeid(Sprite)) { entityManager->RemoveComponent<Sprite>(entity); }
	else if (type == typeid(Tile)) { entityManager->RemoveComponent<Tile>(entity); }
	else if (type == typeid(TileMap)) { entityManager->RemoveComponent<TileMap>(entity); }
	else if (type == typeid(Transform)) { entityManager->RemoveComponent<Transform>(entity); }
	else if (type == typeid(UI_Image)) { entityManager->RemoveComponent<UI_Image>(entity); }
	else
	{
		std::cout << "Error: Invalid component type!" << std::endl;
	}
}

void EntityInspectorGui::CreateAddComponentGui()
{
	ImGui::Text("Add component");

	static std::string selected = "";
	const static std::string names[13] =
	{ 
		"Animated Sprite",
		"Audio",
		"Box Collider",
		"Camera",
		"Circle Collider",
		"Name",
		"Physics",
		"Player Movement",
		"Script Component",
		"Sprite",
		"Tile Map",
		"Transform",
		"UI Image"
	};

	if (ImGui::BeginCombo("New component type", selected.c_str()))
	{
		int noNames = sizeof(names) / sizeof(names[0]);
		for (int i = 0; i < noNames; i++)
		{
			bool isSelected = selected == names[i];
			if (ImGui::Selectable(names[i].c_str(), isSelected))
			{
				selected = names[i];
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Add component"))
	{
		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

		// Components with default/trivial constructors are created directly. Components with higher complexity are instead created through a dialog box
		if (selected == "Animated Sprite") { m_guiObjects.emplace_back(std::make_unique<NewAnimatedSpriteGui>(m_activeEntity)); }
		else if (selected == "Audio") { m_guiObjects.emplace_back(std::make_unique<NewAudioGui>(m_activeEntity)); }
		else if (selected == "Box Collider") { m_guiObjects.emplace_back(std::make_unique<NewBoxColliderGui>(m_activeEntity)); }
		else if (selected == "Camera") { entityManager->AddComponent<Camera>(m_activeEntity, DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT, DEFAULT_NEAR, DEFAULT_FAR, new Framebuffer()); }
		else if (selected == "Circle Collider") { m_guiObjects.emplace_back(std::make_unique<NewCircleColliderGui>(m_activeEntity)); }
		else if (selected == "Name") { entityManager->AddComponent<EntityName>(m_activeEntity, "Entity"); }
		else if (selected == "Physics") { entityManager->AddComponent<Physics>(m_activeEntity); }
		else if (selected == "Player Movement") { entityManager->AddComponent<PlayerMovement>(m_activeEntity); }
		else if (selected == "Script Component") { entityManager->AddComponent<ScriptComponent>(m_activeEntity, entityManager, m_activeEntity); }
		else if (selected == "Sprite") { m_guiObjects.emplace_back(std::make_unique<NewSpriteGui>(m_activeEntity)); }
		else if (selected == "Tile Map") { m_guiObjects.emplace_back(std::make_unique<NewTileMapGui>(m_activeEntity)); }
		else if (selected == "Transform") { entityManager->AddComponent<Transform>(m_activeEntity); }
		else if (selected == "UI Image") { entityManager->AddComponent<UI_Image>(m_activeEntity); }
	}
}

void CreateAnimatedSpriteGui(AnimatedSprite* animatedSprite, Entity owner)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Animated Sprite", nodeFlags))
	{
		bool edited = false;

		// Colour
		glm::vec3 colour;
		{
			colour = animatedSprite->GetColour();
			float colourArr[3] = { colour.x, colour.y, colour.z };
			if (ImGui::DragFloat3("Colour", colourArr, 0.1f, 0.0f, 1.0f, "%.01f"))
			{
				colour.x = colourArr[0];
				colour.y = colourArr[1];
				colour.z = colourArr[2];
				edited = true;
			}
		}

		// Frame interval
		float frameInterval = animatedSprite->GetFrameInterval();
		{
			if (ImGui::DragFloat("Frame interval", &frameInterval, 0.1f, 0.0f))
			{
				edited = true;
			}
		}

		// Frames
		std::string textureName, textureFilePath;
		std::vector<Texture2D> frames = animatedSprite->GetFrames();
		static int selected = 0;
		{
			std::vector<std::string> names;

			if (ImGui::BeginCombo("Frames", frames[selected].m_name.c_str()))
			{
				for (int i = 0; i < frames.size(); i++)
				{
					bool isSelected = frames[selected].m_id == frames[i].m_id;

					std::string text = frames[i].m_name + "##" + std::to_string(i);
					if (ImGui::Selectable(text.c_str(), isSelected))
					{
						selected = i;
					}
				}

				ImGui::EndCombo();
			}

			textureName = frames[selected].m_name;
			textureFilePath = frames[selected].m_filePath;
			if (CreateTextureGui(textureName, textureFilePath))
			{
				edited = true;
			}
		}

		// Shader
		Shader* shader = animatedSprite->GetShader();
		std::string shaderName = shader->m_name;
		std::string vertexPath = shader->m_vertexPath;
		std::string fragmentPath = shader->m_fragmentPath;
		std::string geometryPath = shader->m_geometryPath;
		if (CreateShaderGui(shaderName, vertexPath, fragmentPath, geometryPath))
		{
			edited = true;
		}

		if (edited)
		{
			// Create new frames and shader first; if these fail, don't replace the AnimatedSprite
			std::vector<Texture2D> newFrames;
			for (int i = 0; i < frames.size(); i++)
			{
				std::string name, filePath;
				if (i == selected)
				{
					name = textureName;
					filePath = textureFilePath;
				}
				else
				{
					name = frames[i].m_name;
					filePath = frames[i].m_filePath;
				}

				newFrames.emplace_back(TextureLoader::CreateTexture2DFromFile(name, filePath));
			}

			Shader* newShader = ShaderFactory::CreatePipelineShader(shaderName, vertexPath, fragmentPath, geometryPath);
			if (newShader)
			{
				EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
				entityManager->RemoveComponent<AnimatedSprite>(owner); // Remove old Sprite

				entityManager->AddComponent<AnimatedSprite>(owner, newFrames, frameInterval, colour, newShader);
			}
		}
	}
}

void CreateAudioGui(Audio* audio, Entity owner)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Audio", nodeFlags))
	{
		bool edited = false;

		// File path
		char temp[MAX_INPUT_LENGTH]; // Use to store input
		strcpy_s(temp, audio->m_filePath.length() + 1, audio->m_filePath.c_str());
		std::string filePath;
		if (ImGui::InputText("File", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			filePath = temp;
			edited = true;
		}

		// Channel group (add more as necessary with any additional channel groups)
		FMOD::ChannelGroup* newGroup = audio->m_group;
		{
			std::string previewName;
			FMOD::ChannelGroup* groups[2] = { g_app->m_audioManager->m_sfx, g_app->m_audioManager->m_bgm };
			std::string names[2] = { "sfx", "bgm" };

			if (audio->m_group == g_app->m_audioManager->m_sfx)
			{
				previewName = names[0];
			}
			else
			{
				previewName = names[1];
			}

			if (ImGui::BeginCombo("Channel Group", previewName.c_str()))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = audio->m_group == groups[i];

					if (ImGui::Selectable(names[i].c_str(), isSelected))
					{
						newGroup = groups[i];
						edited = true;
					}
				}

				ImGui::EndCombo();
			}
		}

		if (edited)
		{
			// Create new audio component
			EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
			entityManager->RemoveComponent<Audio>(owner); // Remove old Audio

			FMOD::System* system = g_app->m_audioManager->m_system;
			entityManager->AddComponent<Audio>(owner, filePath.c_str(), system, newGroup);
		}
	}
}

void CreateBoxColliderGui(BoxCollider* boxCollider)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Box Collider", nodeFlags))
	{
		// Size
		{
			glm::vec2 size = boxCollider->m_size;
			float sizeArr[2] = { size.x, size.y };
			if (ImGui::DragFloat2("Size", sizeArr, 0.5f))
			{
				boxCollider->m_size.x = sizeArr[0];
				boxCollider->m_size.y = sizeArr[1];
			}
		}
	}
}

void CreateCameraGui(Camera* camera)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Camera", nodeFlags))
	{
		// Viewport width/height
		{
			float viewportArr[2] = { camera->m_viewportWidth, camera->m_viewportHeight };
			if (ImGui::DragFloat2("Viewport size", viewportArr, 1.0f, 1.0f, 10000.0f))
			{
				camera->m_viewportWidth = viewportArr[0];
				camera->m_viewportHeight = viewportArr[1];
			}
		}

		// Near
		if (ImGui::DragFloat("Near clip", &camera->m_near, 0.1f, 0.0f, 1000.0f)) {}

		// Far
		if (ImGui::DragFloat("Far clip", &camera->m_far, 0.1f, 0.0f, 1000.0f)) {}

		// Active status
		if (camera->m_isActive)
		{
			ImGui::Text("This is the active camera");
		}
		else
		{
			ImGui::Text("This is NOT the active camera");
			if (ImGui::Button("Set active"))
			{
				EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
				g_app->SetActiveCamera(entityManager->GetEntityFromComponent<Camera>(camera));
			}
		}
	}
}

void CreateCircleColliderGui(CircleCollider* circleCollider)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Circle Collider", nodeFlags))
	{
		// Centre
		{
			glm::vec2 centre = circleCollider->m_centre;
			float centreArr[2] = { centre.x, centre.y };
			if (ImGui::DragFloat2("Centre", centreArr, 0.1f))
			{
				circleCollider->m_centre.x = centreArr[0];
				circleCollider->m_centre.y = centreArr[1];
			}
		}

		// Radius
		if (ImGui::DragFloat("Radius", &circleCollider->m_radius, 0.1f, 0.1f, 1000.0f)) {}
	}
}

void CreateEntityNameGui(EntityName* entityName)
{
	char temp[MAX_INPUT_LENGTH];
	strcpy_s(temp, entityName->m_name.length() + 1, entityName->m_name.c_str());
	if (ImGui::InputText("Entity name", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		entityName->m_name = temp;
	}
}

void CreatePhysicsGui(Physics* physics)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Physics", nodeFlags))
	{
		// Mass
		if (ImGui::DragFloat("Mass", &physics->m_mass, 0.1f, 0.0f, 1000.0f)) {}
	}
}

void CreateUI_ImageGui(UI_Image* image, Entity owner)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("UI Image", nodeFlags))
	{
		ImGui::InputFloat("Relative Pos X", &image->m_position.x);
		ImGui::InputFloat("Absolute Pos X", &image->m_position.y);
		ImGui::InputFloat("Relative Pos Y", &image->m_position.z);
		ImGui::InputFloat("Absolute Pos Y", &image->m_position.w);
		ImGui::InputFloat("Relative Size X", &image->m_size.x);
		ImGui::InputFloat("Absolute Size X", &image->m_size.y);
		ImGui::InputFloat("Relative Size Y", &image->m_size.z);
		ImGui::InputFloat("Absolute Size Y", &image->m_size.w);
		ImGui::InputInt("Z Index", &image->m_zIndex);

		// Texture
		bool edited = false;
		Texture2D texture = image->m_texture;
		std::string textureName = texture.m_name;
		std::string textureFilePath = texture.m_filePath;
		if (CreateTextureGui(textureName, textureFilePath))
		{
			edited = true;
		}

		if (edited)
		{
			// Create new texture and shader first; if these fail, don't replace the Sprite
			Texture2D newTexture = TextureLoader::CreateTexture2DFromFile(textureName, textureFilePath);
			if (newTexture.m_id != 0)
			{
				EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
				entityManager->RemoveComponent<UI_Image>(owner); // Remove old Sprite
				entityManager->AddComponent<UI_Image>(owner, newTexture, image->m_position, image->m_size, image->m_zIndex);
			}
		}
	}
}

void CreatePlayerMovementGui(PlayerMovement* playerMovement)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Player Movement", nodeFlags))
	{
		// Movement speed
		if (ImGui::DragFloat("Speed", &playerMovement->m_speed, 0.1f, 0.0f, 1000.0f)) {}
	}
}

void CreateScriptComponentGui(ScriptComponent* scriptComponent)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Script", nodeFlags))
	{
		ImGui::Text("This entity has a script component. Editor script interaction is not currently a feature.");
	}
}

void CreateSpriteGui(Sprite* sprite, Entity owner)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Sprite", nodeFlags))
	{
		bool edited = false;

		// Colour
		glm::vec3 colour = sprite->GetColour();
		float colourArr[3] = { colour.x, colour.y, colour.z };
		if (ImGui::DragFloat3("Colour", colourArr, 0.1f, 0.0f, 1.0f, "%.01f"))
		{
			edited = true;
		}

		// Shader
		Shader* shader = sprite->GetShader();
		std::string shaderName = "";
		std::string vertexPath = "";
		std::string fragmentPath = "";
		std::string geometryPath = "";
		if (shader) // If shader exists, use existing values in input textboxes
		{
			shaderName = shader->m_name;
			vertexPath = shader->m_vertexPath;
			fragmentPath = shader->m_fragmentPath;
			geometryPath = shader->m_geometryPath;
		}
		if (CreateShaderGui(shaderName, vertexPath, fragmentPath, geometryPath))
		{
			edited = true;
		}

		// Texture
		Texture2D texture = sprite->GetTexture();
		std::string textureName = texture.m_name;
		std::string textureFilePath = texture.m_filePath;
		if (CreateTextureGui(textureName, textureFilePath))
		{
			edited = true;
		}

		if (edited)
		{
			// Create new texture and shader first; if these fail, don't replace the Sprite
			Texture2D newTexture = TextureLoader::CreateTexture2DFromFile(textureName, textureFilePath);
			Shader* newShader = ShaderFactory::CreatePipelineShader(shaderName, vertexPath, fragmentPath, geometryPath);
			if (newTexture.m_id != 0)
			{
				EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
				entityManager->RemoveComponent<Sprite>(owner); // Remove old Sprite

				glm::vec3 newColour = glm::vec3(colourArr[0], colourArr[1], colourArr[2]);
				entityManager->AddComponent<Sprite>(owner, newTexture, newColour, newShader);
			}
		}
	}
}

void CreateTileGui(Tile* tile)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Tile", nodeFlags))
	{
		// Tile type (add more strings as necessary with new added types)
		{
			std::string previewName;
			std::string names[3] = { "None", "House", "Tree" };

			switch (tile->m_object)
			{
			case TileObject::NONE:
				previewName = "None";
				break;
			case TileObject::HOUSE:
				previewName = "House";
				break;
			case TileObject::TREE:
				previewName = "Tree";
				break;
			}

			if (ImGui::BeginCombo("##combo", previewName.c_str()))
			{
				for (int i = 0; i < 3; i++)
				{
					bool isSelected = (int)tile->m_object == i;

					if (ImGui::Selectable(names[i].c_str(), isSelected))
					{
						tile->m_object = (TileObject)i;
					}
				}

				ImGui::EndCombo();
			}
		}
	}
}

void CreateTileMapGui(TileMap* tileMap)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Tile Map", nodeFlags))
	{
		// Tile size
		if (ImGui::DragFloat2("Tile size", glm::value_ptr(tileMap->m_tileSize), 0.5f)) {}

		// Map size
		if (ImGui::DragFloat2("Map size", glm::value_ptr(tileMap->m_mapSize), 0.5f)) {}
	}
}

void CreateTransformGui(Transform* transform)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Transform", nodeFlags))
	{
		// Position
		{
			glm::vec2 pos = transform->m_position;
			float posArr[2] = { pos.x, pos.y };
			if (ImGui::DragFloat2("Position", posArr, 0.5f))
			{
				transform->m_position.x = posArr[0];
				transform->m_position.y = posArr[1];
			}
		}

		// Rotation
		if (ImGui::DragFloat("Rotation", &transform->m_rotate, 0.5f, 0.0f, 360.0f)) {}

		// Size
		{
			glm::vec2 size = transform->m_size;
			float sizeArr[2] = { size.x, size.y };
			if (ImGui::DragFloat2("Size", sizeArr, 0.1f, 0.1f, 100.0f))
			{
				transform->m_size.x = sizeArr[0];
				transform->m_size.y = sizeArr[1];
			}
		}
	}
}

bool CreateShaderGui(std::string& shaderName, std::string& vertexPath, std::string& fragmentPath, std::string& geometryPath)
{
	bool edited = false;
	char temp[MAX_INPUT_LENGTH]; // Use to store input

	// Shader name
	{
		strcpy_s(temp, shaderName.length() + 1, shaderName.c_str());
		if (ImGui::InputText("Shader name##", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			shaderName = temp;
			edited = true;
		}
	}

	// Vertex path
	{
		strcpy_s(temp, vertexPath.length() + 1, vertexPath.c_str());
		if (ImGui::InputText("Vertex path##", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			vertexPath = temp;
			edited = true;
		}
	}

	// Fragment path
	{
		strcpy_s(temp, fragmentPath.length() + 1, fragmentPath.c_str());
		if (ImGui::InputText("Fragment path##", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			fragmentPath = temp;
			edited = true;
		}
	}

	// Geometry path
	if (!geometryPath.empty())
	{
		strcpy_s(temp, geometryPath.length() + 1, geometryPath.c_str());
		if (ImGui::InputText("Geometry path##", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			geometryPath = temp;
			edited = true;
		}
	}

	return edited;
}

bool CreateTextureGui(std::string& textureName, std::string& filePath)
{
	bool edited = false;
	char temp[MAX_INPUT_LENGTH]; // Use to store input

	// Texture name
	{
		strcpy_s(temp, textureName.length() + 1, textureName.c_str());
		if (ImGui::InputText("Texture name##", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			textureName = temp;
			edited = true;
		}
	}

	// File path
	{
		strcpy_s(temp, filePath.length() + 1, filePath.c_str());
		if (ImGui::InputText("File path##", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			filePath = temp;
			edited = true;
		}
	}

	return edited;
}