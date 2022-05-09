#include "CreateSceneGui.h"

#include <ImGui/imgui.h>

CreateSceneGui::CreateSceneGui() :
	inputName("")
{}

void CreateSceneGui::CreateGui()
{
	ImGui::OpenPopup("Create scene");

	// Center popup?
	/*ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos(io.DisplaySize.x / 2.0f, io.DisplaySize.y / 2.0f);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));*/

	if (ImGui::BeginPopupModal("Create scene"))
	{
		ImGui::Text("Make sure you've exported the current scene!\nUnsaved changes will be deleted.");
		ImGui::Separator();

		if (ImGui::InputText("Scene name##", inputName, IM_ARRAYSIZE(inputName))) {}

		if (ImGui::Button("Create scene##"))
		{
			std::string activeSceneName = g_app->m_sceneManager->GetActiveSceneName();
			std::string nameString(inputName);

			bool success = g_app->m_sceneManager->CreateScene(nameString);
			if (success)
			{
				g_app->m_sceneManager->SetActiveScene(nameString);
				g_app->m_sceneManager->DestroyScene(activeSceneName);

				close = true;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel##")) { close = true; ImGui::CloseCurrentPopup(); }

		ImGui::EndPopup();
	}
}