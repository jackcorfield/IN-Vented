#include "NewCircleColliderGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewCircleColliderGui::NewCircleColliderGui(Entity entityFor) :
	entity(entityFor),
	inputRadius(1.0f),
	inputCentrePos()
{}

void NewCircleColliderGui::CreateGui()
{
	ImGui::OpenPopup("New circle collider");

	if (ImGui::BeginPopupModal("New circle collider"))
	{
		if (ImGui::DragFloat("Radius", &inputRadius, 0.5f)) {}
		if (ImGui::DragFloat2("Centre pos", glm::value_ptr(inputCentrePos), 0.5f)) {}

		if (ImGui::Button("Add circle collider##"))
		{
			AddCircleCollider();
			close = true;
			ImGui::CloseCurrentPopup();
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

void NewCircleColliderGui::AddCircleCollider()
{
	g_app->m_sceneManager->GetActiveScene()->m_entityManager->AddComponent<CircleCollider>(entity, inputRadius, inputCentrePos);
}