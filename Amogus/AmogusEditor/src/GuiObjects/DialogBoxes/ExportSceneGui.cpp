#include "ExportSceneGui.h"

#include <ImGui/imgui.h>

ExportSceneGui::ExportSceneGui() :
	inputFilePath("")
{}

void ExportSceneGui::CreateGui()
{
	ImGui::OpenPopup("Export scene");

	if (ImGui::BeginPopupModal("Export scene"))
	{
		if (ImGui::InputText("Scene filename##", inputFilePath, IM_ARRAYSIZE(inputFilePath))) {}

		if (ImGui::Button("Export scene##"))
		{
			std::string activeSceneName = g_app->m_sceneManager->GetActiveSceneName();
			std::string filePathString(inputFilePath);

			bool success = SceneExporter::ExportActiveSceneToFile(filePathString);
			if (success)
			{
				close = true;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel##")) { close = true; ImGui::CloseCurrentPopup(); }

		ImGui::EndPopup();
	}
}