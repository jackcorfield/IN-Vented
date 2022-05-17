#include "CreateSceneGui.h"

#include <ImGui/imgui.h>
#include "../../ImGuiLayer.h"

CreateSceneGui::CreateSceneGui(ImGuiLayer* layer) :
	IGuiObject(layer), 
	m_inputName("")
{}

void CreateSceneGui::CreateGui()
{
	ImGui::OpenPopup("Create scene");

	if (ImGui::BeginPopupModal("Create scene"))
	{
		ImGui::Text("Make sure you've exported the current scene!\nUnsaved changes will be deleted.");
		ImGui::Separator();

		ImGui::InputText("Scene Name", m_inputName, 128);
		ImGui::ColorEdit3("Scene Colour", col);

		if (ImGui::Button("Create"))
		{
			layer->CreateScene(m_inputName, col);
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