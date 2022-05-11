#include "NewGameGui.h"

#include <ImGui/imgui.h>
#include "../../ImGuiLayer.h"

NewGameGui::NewGameGui(ImGuiLayer* layer) : 
	IGuiObject(layer),
	m_inputName("")
{
}

void NewGameGui::CreateGui()
{
	ImGui::OpenPopup("New Game");

	if (ImGui::BeginPopupModal("New Game"))
	{
		ImGui::Text("Make sure you've saved the current game!\nUnsaved changes will be deleted.");
		ImGui::Separator();

		ImGui::InputText("Name", m_inputName, 128);
		if (ImGui::Button("Create"))
		{
			layer->CreateGame(m_inputName);
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