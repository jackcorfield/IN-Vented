#include "LoadGameGui.h"

#include <ImGui/imgui.h>
#include "../../ImGuiLayer.h"

LoadGameGui::LoadGameGui(ImGuiLayer* layer) :
	IGuiObject(layer),
	m_inputName("")
{
}

void LoadGameGui::CreateGui()
{
	ImGui::OpenPopup("Load Game");

	if (ImGui::BeginPopupModal("Load Game"))
	{
		ImGui::Text("Make sure you've saved the current game!\nUnsaved changes will be deleted.");
		ImGui::Separator();

		ImGui::InputText("Name", m_inputName, IM_ARRAYSIZE(m_inputName));
		if (ImGui::Button("Load"))
		{
			layer->LoadGame(m_inputName);
			ImGui::CloseCurrentPopup();
			close = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			close = true;
		}

		ImGui::EndPopup();
	}
}