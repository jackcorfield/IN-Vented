#include "ImportSceneGui.h"

#include <ImGui/imgui.h>

#define MAX_RECENT_SCENES 5

ImportSceneGui::ImportSceneGui(std::list<std::string>* recentScenes) :
	m_inputFilePath(""),
	p_recentScenes(recentScenes)
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

		if (ImGui::InputText("Scene filename##", m_inputFilePath, IM_ARRAYSIZE(m_inputFilePath))) {}

		if (ImGui::Button("Import scene##"))
		{
			std::string activeSceneName = g_app->m_sceneManager->GetActiveSceneName();
			std::string filePathString(m_inputFilePath);

			bool success = SceneImporter::ImportSceneFromFile(filePathString, true);
			if (success)
			{
				p_recentScenes->erase(std::remove(p_recentScenes->begin(), p_recentScenes->end(), g_app->m_sceneManager->GetActiveSceneName()), p_recentScenes->end()); // Remove possible previous instance of this name (thus moving it to the front)
				p_recentScenes->push_front(g_app->m_sceneManager->GetActiveSceneName());
				if (p_recentScenes->size() > MAX_RECENT_SCENES)
				{
					p_recentScenes->pop_back();
				}

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