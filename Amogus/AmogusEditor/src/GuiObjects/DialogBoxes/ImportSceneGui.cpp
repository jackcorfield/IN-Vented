#include "ImportSceneGui.h"

#include <ImGui/imgui.h>
#include "../../ImGuiLayer.h"

#define MAX_RECENT_SCENES 5

ImportSceneGui::ImportSceneGui(ImGuiLayer* layer, std::list<std::string>* recentScenes) :
	IGuiObject(layer),
	m_inputName(""),
	p_recentScenes(recentScenes)
{}

void ImportSceneGui::CreateGui()
{
	ImGui::OpenPopup("Import scene");

	if (ImGui::BeginPopupModal("Import scene"))
	{
		ImGui::Text("Make sure you've exported the current scene!\nUnsaved changes will be deleted.");
		ImGui::Separator();

		ImGui::InputText("Scene Name", m_inputName, IM_ARRAYSIZE(m_inputName));

		if (ImGui::Button("Load"))
		{
			if (layer->LoadScene(m_inputName))
			{
				p_recentScenes->erase(std::remove(p_recentScenes->begin(), p_recentScenes->end(), m_inputName), p_recentScenes->end()); // Remove possible previous instance of this name (thus moving it to the front)
				p_recentScenes->push_front(g_app->m_sceneManager->GetActiveSceneName());
				if (p_recentScenes->size() > MAX_RECENT_SCENES)
				{
					p_recentScenes->pop_back();
				}

				close = true;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { close = true; ImGui::CloseCurrentPopup(); }

		ImGui::EndPopup();
	}
}