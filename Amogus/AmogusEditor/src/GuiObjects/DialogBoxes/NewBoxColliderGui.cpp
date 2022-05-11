#include "NewBoxColliderGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewBoxColliderGui::NewBoxColliderGui(Entity entityFor) :
	IGuiObject(nullptr),
	entity(entityFor),
	inputSize()
{}

void NewBoxColliderGui::CreateGui()
{
	ImGui::OpenPopup("New box collider");

	if (ImGui::BeginPopupModal("New box collider"))
	{
		if (ImGui::DragFloat2("Size", glm::value_ptr(inputSize), 0.5f)) {}

		if (ImGui::Button("Add box collider##"))
		{
			AddBoxCollider();
			ImGui::CloseCurrentPopup();
			close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			close = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void NewBoxColliderGui::AddBoxCollider()
{
	g_app->m_sceneManager->GetActiveScene()->m_entityManager->AddComponent<BoxCollider>(entity, inputSize);
}