#include "EntityInspectorGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <string.h>

#include <Amogus.h>

#include "DialogBoxes/EditAnimationsGui.h"
#include "DialogBoxes/NewAnimatedSpriteGui.h"
#include "DialogBoxes/NewAudioGui.h"
#include "DialogBoxes/NewBoxColliderGui.h"
#include "DialogBoxes/NewCircleColliderGui.h"
#include "DialogBoxes/NewSpriteGui.h"
#include "DialogBoxes/NewTileMapGui.h"

#define MAX_INPUT_LENGTH 512

#define DEFAULT_VIEWPORT_WIDTH 1280
#define DEFAULT_VIEWPORT_HEIGHT 720
#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 1.0f

void DrawComponentGui(void* component, std::type_index type, Entity entity, EntityManager* entityManager, int i, std::unique_ptr<IGuiObject>& popupPtr);
void DeleteComponent(void* component, std::type_index type, Entity entity, EntityManager* entityManager);

/// Add prototypes here for new components (and define below with the others) ///
// Inspector gui functions
void CreateAnimatedSpriteGui(AnimatedSprite* animatedSprite, Entity owner, std::unique_ptr<IGuiObject>& popupPtr);
void CreateAudioGui(Audio* m_audio, Entity owner);
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
void CreateUI_WidgetComponentGui(UI_WidgetComponent* widget, Entity owner);

// Helpers (return true if changed)
bool CreateShaderGui(std::string& shaderName, std::string& vertexPath, std::string& fragmentPath, std::string& geometryPath);
bool CreateTextureGui(std::string& textureName, std::string& filePath, int optionalId = -1, std::string optionalTitlePrefix = "");

EntityInspectorGui::EntityInspectorGui() :
	m_activeEntity(0)
{}

EntityInspectorGui::~EntityInspectorGui()
{
	if (m_popup) { m_popup.release(); }
}

void EntityInspectorGui::Draw()
{
	ImGui::Begin("Inspector");

	if (m_activeEntity != 0)
	{
		DrawInspectorInfo();
	}

	if (m_popup)
	{
		m_popup.get()->CreateGui();

		if (m_popup.get()->close) { m_popup.release(); }
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

		DrawComponentGui(component, typeIndex, m_activeEntity, entityManager, i, m_popup);

		ImGui::Separator();

		i++;
	}

	CreateAddComponentGui();
}

void EntityInspectorGui::SetActiveEntity(Entity entity)
{
	m_activeEntity = entity;
}

void DrawComponentGui(void* component, std::type_index type, Entity entity, EntityManager* entityManager, int i, std::unique_ptr<IGuiObject>& popupPtr)
{
	if (!component)
	{
		return;
	}

	if (type == typeid(AnimatedSprite)) { CreateAnimatedSpriteGui(reinterpret_cast<AnimatedSprite*>(component), entity, popupPtr); }
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
	else if (type == typeid(UI_WidgetComponent)) { CreateUI_WidgetComponentGui(reinterpret_cast<UI_WidgetComponent*>(component), entity); }
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
	else if (type == typeid(UI_WidgetComponent)) { entityManager->RemoveComponent<UI_WidgetComponent>(entity); }
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
		"UI Widget"
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
		if (selected == "Animated Sprite") { m_popup = std::make_unique<NewAnimatedSpriteGui>(m_activeEntity); }
		else if (selected == "Audio") { m_popup = std::make_unique<NewAudioGui>(m_activeEntity); }
		else if (selected == "Box Collider") { m_popup = std::make_unique<NewBoxColliderGui>(m_activeEntity); }
		else if (selected == "Camera") { entityManager->AddComponent<Camera>(m_activeEntity, DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT, DEFAULT_NEAR, DEFAULT_FAR, new Framebuffer()); }
		else if (selected == "Circle Collider") { m_popup = std::make_unique<NewCircleColliderGui>(m_activeEntity); }
		else if (selected == "Name") { entityManager->AddComponent<EntityName>(m_activeEntity, "Entity"); }
		else if (selected == "Physics") { entityManager->AddComponent<Physics>(m_activeEntity); }
		else if (selected == "Player Movement") { entityManager->AddComponent<PlayerMovement>(m_activeEntity); }
		else if (selected == "Script Component") { entityManager->AddComponent<ScriptComponent>(m_activeEntity, entityManager, m_activeEntity); }
		else if (selected == "Sprite") { m_popup = std::make_unique<NewSpriteGui>(m_activeEntity); }
		else if (selected == "Tile Map") { m_popup = std::make_unique<NewTileMapGui>(m_activeEntity); }
		else if (selected == "Transform") { entityManager->AddComponent<Transform>(m_activeEntity); }
		else if (selected == "UI Widget") { entityManager->AddComponent<UI_WidgetComponent>(m_activeEntity); }
	}
}

void CreateAnimatedSpriteGui(AnimatedSprite* animatedSprite, Entity owner, std::unique_ptr<IGuiObject>& popupPtr)
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

		// FrameSize
		glm::vec2 frameSize;
		{
			frameSize = animatedSprite->getFrameSize();
			int frameSizeArr[2] = { frameSize.x, frameSize.y };
			if (ImGui::DragInt2("Frame Size", frameSizeArr))
			{
				frameSize.x = frameSizeArr[0];
				frameSize.y = frameSizeArr[1];
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

		// Texture
		Texture2D texture = animatedSprite->GetTexture();
		std::string textureName = texture.m_name;
		std::string textureFilePath = texture.m_filePath;
		if (CreateTextureGui(textureName, textureFilePath))
		{
			edited = true;
		}

		// Animation selection for visual testing
		static std::string preview = "##";
		
		if (ImGui::BeginCombo("Animation tests", preview.c_str()))
		{
			std::map<std::string, Animation>& allAnimations = animatedSprite->getAnimations();
			for (auto i = allAnimations.begin(); i != allAnimations.end(); i++)
			{
				bool isSelected = preview == i->first;
				if (ImGui::Selectable(i->first.c_str(), isSelected))
				{
					preview = i->first;
					animatedSprite->setAnimation(i->first);
				}
			}

			ImGui::EndCombo();
		}

		// Open edit animations dialog
		if (ImGui::Button("Edit animations..."))
		{
			popupPtr = std::make_unique<EditAnimationsGui>(owner);
		}

		if (edited)
		{
			
		}
	}
}

void CreateAudioGui(Audio* m_audio, Entity owner)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Audio", nodeFlags))
	{
		bool edited = false;

		// File path
		char temp[MAX_INPUT_LENGTH]; // Use to store input
		strcpy_s(temp, m_audio->m_filePath.length() + 1, m_audio->m_filePath.c_str());
		std::string filePath;
		if (ImGui::InputText("File", temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			filePath = temp;
			edited = true;
		}

		// Channel group (add more as necessary with any additional channel groups)
		FMOD::ChannelGroup* newGroup = m_audio->m_group;
		{
			std::string previewName;
			FMOD::ChannelGroup* groups[2] = { g_app->m_audioManager->m_sfx, g_app->m_audioManager->m_bgm };
			std::string names[2] = { "sfx", "bgm" };

			if (m_audio->m_group == g_app->m_audioManager->m_sfx)
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
					bool isSelected = m_audio->m_group == groups[i];

					if (ImGui::Selectable(names[i].c_str(), isSelected))
					{
						newGroup = groups[i];
						edited = true;
					}
				}

				ImGui::EndCombo();
			}
		}

		if (ImGui::Button("Play audio"))
		{
			g_app->m_audioManager->PlayAudio(m_audio->m_sound, m_audio->m_group, m_audio->m_channel);
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

		// Offset
		{
			glm::vec2 offset = boxCollider->m_offset;
			if (ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.5f)) {}
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

		// Internal viewport width/height
		{
			float internalViewportArr[2] = { camera->m_internalWidth, camera->m_internalHeight };
			if (ImGui::DragFloat2("Internal viewport size", internalViewportArr, 1.0f, 1.0f, 10000.0f))
			{
				camera->m_internalWidth = internalViewportArr[0];
				camera->m_internalHeight = internalViewportArr[1];
			}
		}

		// Near
		if (ImGui::DragFloat("Near clip", &camera->m_near, 0.1f)) {}

		// Far
		if (ImGui::DragFloat("Far clip", &camera->m_far, 0.1f)) {}

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

void CreateUI_WidgetComponentGui(UI_WidgetComponent* widget, Entity owner)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("UI Widget", nodeFlags))
	{
		static std::string selected = "";
		if (ImGui::BeginCombo("New UI Element", selected.c_str()))
		{
			for (int i = 0; i < NumElementTypes; i++)
			{
				if (i == ET_Base) continue;

				bool isSelected = selected == s_ElementType[i];
				if (ImGui::Selectable(s_ElementType[i].c_str(), isSelected))
				{
					selected = s_ElementType[i];
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button("Create UI Element"))
		{
			for (int i = 0; i < NumElementTypes; i++)
			{
				if (s_ElementType[i] == selected)
				{
					widget->AddElement(i);
				}
			}
		}

		for (int i = 0; i < widget->m_elements.size(); i++)
		{
			std::string sIdx = std::to_string(i);
			UI_BaseElement* element = widget->m_elements[i];
			if (ImGui::CollapsingHeader(element->m_name.c_str(), nodeFlags))
			{
				std::string nameCopy = element->m_name;
				if (ImGui::InputText((std::string("Name##") + sIdx).c_str(), &nameCopy[0], MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					bool used = false;
					for (UI_BaseElement* e : widget->m_elements)
					{
						if (e == element) continue;
						if (e->m_name == nameCopy)
						{
							used = true;
						}
					}

					if (!used)
					{
						element->m_name = nameCopy;
					}
				}

				float abPosArr[2] = { element->m_absolutePosition.x, element->m_absolutePosition.y };
				if (ImGui::InputFloat2((std::string("Absolute Position##")+sIdx).c_str(), abPosArr))
				{
					element->m_absolutePosition = glm::vec2(abPosArr[0], abPosArr[1]);
				}

				float abSizeArr[2] = { element->m_absoluteSize.x, element->m_absoluteSize.y };
				if (ImGui::InputFloat2((std::string("Absolute Size##") + sIdx).c_str(), abSizeArr))
				{
					element->m_absoluteSize = glm::vec2(abSizeArr[0], abSizeArr[1]);
				}

				float resPosArr[2] = { element->m_relativePosition.x, element->m_relativePosition.y };
				if (ImGui::InputFloat2((std::string("Relative Position##") + sIdx).c_str(), resPosArr))
				{
					element->m_relativePosition = glm::vec2(resPosArr[0], resPosArr[1]);
				}

				float resSizeArr[2] = { element->m_relativeSize.x, element->m_relativeSize.y };
				if (ImGui::InputFloat2((std::string("Relative Size##") + sIdx).c_str(), resSizeArr))
				{
					element->m_relativeSize = glm::vec2(resSizeArr[0], resSizeArr[1]);
				}

				float colourArr[3] = { element->m_colour.r, element->m_colour.g, element->m_colour.b };
				if (ImGui::ColorPicker3((std::string("Colour##") + sIdx).c_str(), colourArr))
				{
					element->m_colour = glm::vec3(colourArr[0], colourArr[1], colourArr[2]);
				}

				ImGui::Checkbox((std::string("Hidden##") + sIdx).c_str(), &element->m_hidden);
				ImGui::InputInt((std::string("Z Index##") + sIdx).c_str(), &element->m_zIndex);

				// Texture
				if (element->m_elementType == ET_Image || element->m_elementType == ET_ImageButton)
				{
					UI_Image* imageElement = (UI_Image*)element;
					Texture2D texture = imageElement->m_texture;
					std::string textureName = texture.m_name;
					std::string textureFilePath = texture.m_filePath;
					if (CreateTextureGui(textureName, textureFilePath, i))
					{
						imageElement->m_texture = TextureLoader::CreateTexture2DFromFile(textureName, textureFilePath);
					}
				}

				if (element->m_elementType == ET_ImageButton)
				{
					UI_ImageButton* buttonElement = (UI_ImageButton*)element;
					Texture2D htexture = buttonElement->m_hoveredTexture;
					std::string htextureName = htexture.m_name;
					std::string htextureFilePath = htexture.m_filePath;
					if (CreateTextureGui(htextureName, htextureFilePath, i, "Hover "))
					{
						buttonElement->m_hoveredTexture = TextureLoader::CreateTexture2DFromFile(htextureName, htextureFilePath);
					}
;
					Texture2D ctexture = buttonElement->m_clickedTexture;
					std::string ctextureName = ctexture.m_name;
					std::string ctextureFilePath = ctexture.m_filePath;
					if (CreateTextureGui(ctextureName, ctextureFilePath, i, "Click "))
					{
						buttonElement->m_clickedTexture = TextureLoader::CreateTexture2DFromFile(ctextureName, ctextureFilePath);
					}
				}

				if (element->m_elementType == ET_Text)
				{
					UI_Text* textElement = (UI_Text*)element;

					ImGui::Checkbox((std::string("Centered##") + sIdx).c_str(), &textElement->m_centered);

					char* buffer = &textElement->m_text[0];
					ImGui::InputText((std::string("Text##") + sIdx).c_str(), buffer, MAX_INPUT_LENGTH);
					textElement->m_text = std::string(buffer);
				}

				if (ImGui::Button((std::string("Delete element##") + sIdx).c_str()))
				{
					widget->RemoveElement(element->m_name);
				}
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

		// Depth
		if (ImGui::DragFloat("Depth", &transform->m_depth, 0.1f, -1.0f, 1.0f)) {}
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

bool CreateTextureGui(std::string& textureName, std::string& filePath, int optionalId, std::string optionalTitlePrefix)
{
	bool edited = false;
	char temp[MAX_INPUT_LENGTH]; // Use to store input
	std::string optionalIdString = std::to_string(optionalId);

	// Texture name
	{
		strcpy_s(temp, textureName.length() + 1, textureName.c_str());
		if (ImGui::InputText((optionalTitlePrefix + std::string("Texture name##") + optionalIdString).c_str(), temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			textureName = temp;
			edited = true;
		}
	}

	// File path
	{
		strcpy_s(temp, filePath.length() + 1, filePath.c_str());
		if (ImGui::InputText((optionalTitlePrefix + std::string("File path##") + optionalIdString).c_str(), temp, MAX_INPUT_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			filePath = temp;
			edited = true;
		}
	}

	return edited;
}