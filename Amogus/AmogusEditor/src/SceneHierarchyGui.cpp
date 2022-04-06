#include "SceneHierarchyGui.h"

#include "EntityInspectorGui.h"
#include <imgui/imgui.h>
#include <string>

void SceneHierarchyGui::Draw()
{
	ImGui::Begin("Hierarchy");

	std::vector<Entity> allEntities = m_activeScene->m_entityManager->GetAllActiveEntities();

	Entity selectedEntity = 0;

	for (size_t i = 0; i < allEntities.size(); i++)
	{
		if (DrawEntityNode(allEntities[i], (int)i))
		{
			selectedEntity = allEntities[i];
		}
	}

	ImGui::End();

	if (selectedEntity != 0)
	{
		m_selectedEntity = selectedEntity;
		m_entityInspector->SetActiveEntity(selectedEntity);
	}
}

void SceneHierarchyGui::SetCurrentScene(Scene* scene)
{
	m_activeScene = scene;
}

void SceneHierarchyGui::SetEntityInspector(EntityInspectorGui* entityInspector)
{
	m_entityInspector = entityInspector;
}

bool SceneHierarchyGui::DrawEntityNode(Entity entity, int i)
{
	std::string label;
	if (m_activeScene->m_entityManager->HasComponent<EntityName>(entity))
	{
		label = m_activeScene->m_entityManager->GetComponent<EntityName>(entity)->m_name;
	}
	else
	{
		label = "Entity " + std::to_string(i);
	}

	ImGuiTreeNodeFlags nodeFlags = (i + 1 == m_selectedEntity ? ImGuiTreeNodeFlags_Selected : 0); // Add 1 to cover (i == 0), because 0 is the null value for entities
	nodeFlags |= ImGuiTreeNodeFlags_Leaf; // Use leaf for entities with no children (which is currently all of them)
	//nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;  // Only open nodes with the arrow icon (allows clicking elsewhere to just select it) (this is here for when entity hierarchy is fully implemented)

	bool nodeIsOpen = ImGui::TreeNodeEx(label.c_str(), nodeFlags); // Separate from if statement so we can check for click separately

	bool clicked = false;
	if (ImGui::IsItemClicked())
	{
		clicked = true;
	}

	if (nodeIsOpen)
	{
		/*std::map<std::type_index, void*> entityComponents = m_activeScene->m_entityManager->GetAllComponents(entity);
		for (const auto& component : entityComponents)
		{
			if (DrawComponentNode(component.second, component.first, i))
			{
				clicked = true;
			}
		}*/

		ImGui::TreePop();
	}

	return clicked;
}

//bool SceneHierarchyGui::DrawComponentNode(void* component, std::type_index type, int i)
//{
//	std::string selectLabel;
//
//	if (type == typeid(Sprite))
//	{
//		selectLabel = "Sprite##";
//	}
//	else if (type == typeid(AnimatedSprite))
//	{
//		selectLabel = "Animated Sprite##";
//	}
//	else if (type == typeid(Camera))
//	{
//		selectLabel = "Camera##";
//	}
//	else if (type == typeid(Audio))
//	{
//		selectLabel = "Audio##";
//	}
//	else if (type == typeid(Transform))
//	{
//		selectLabel = "Transform##";
//	}
//	else if (type == typeid(Physics))
//	{
//		selectLabel = "Physics##";
//	}
//	else if (type == typeid(PlayerMovement))
//	{
//		selectLabel = "Player Movement##";
//	}
//	else if (type == typeid(BoxCollider))
//	{
//		selectLabel = "Box Coliider##";
//	}
//	else
//	{
//		selectLabel = "Default##";
//	}
//
//	selectLabel += std::to_string(i);
//
//	ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf;
//
//	bool clicked = false;
//	if (ImGui::TreeNodeEx(selectLabel.c_str(), leafFlags))
//	{
//		if (ImGui::IsItemClicked())
//		{
//			clicked = true;
//		}
//
//		ImGui::TreePop();
//	}
//
//	return clicked;
//}