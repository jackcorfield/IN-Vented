#include "NewBoxColliderGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewBoxColliderGui::NewBoxColliderGui(Entity entityFor) :
	entity(entityFor),
	inputPos(),
	inputSize()
{}

void NewBoxColliderGui::CreateGui()
{
	if (ImGui::Begin("New box collider"))
	{
		if (ImGui::DragFloat2("Pos", glm::value_ptr(inputPos), 0.5f)) {}
		if (ImGui::DragFloat2("Size", glm::value_ptr(inputSize), 0.5f)) {}

		if (ImGui::Button("Add box collider##"))
		{
			AddBoxCollider();
			close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			close = true;
		}
	}

	ImGui::End();
}

void NewBoxColliderGui::AddBoxCollider()
{
	g_app->m_sceneManager->GetActiveScene()->m_entityManager->AddComponent<BoxCollider>(entity, inputPos, inputSize);
}