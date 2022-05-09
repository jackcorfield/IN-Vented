#include "ImportSceneGui.h"

#include <ImGui/imgui.h>

ImportSceneGui::ImportSceneGui() :
	inputFilePath("")
{}

void ImportSceneGui::CreateGui()
{
	ImGui::OpenPopup("Import scene");

	// Center popup?
	/*ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos(io.DisplaySize.x / 2.0f, io.DisplaySize.y / 2.0f);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));*/

	if (ImGui::BeginPopupModal("Import scene"))
	{
		ImGui::Text("Make sure you've exported the current scene!\nUnsaved changes will be deleted.");
		ImGui::Separator();

		if (ImGui::InputText("Scene filename##", inputFilePath, IM_ARRAYSIZE(inputFilePath))) {}

		if (ImGui::Button("Import scene##"))
		{
			std::string activeSceneName = g_app->m_sceneManager->GetActiveSceneName();
			std::string filePathString(inputFilePath);

			bool success = SceneImporter::ImportSceneFromFile(filePathString, true);
			if (success)
			{
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