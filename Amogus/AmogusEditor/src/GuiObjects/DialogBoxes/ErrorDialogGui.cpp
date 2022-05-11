#include "ErrorDialogGui.h"

#include <ImGui/imgui.h>
#include "../../ImGuiLayer.h"

ErrorDialogGui::ErrorDialogGui(const char* error) :
	IGuiObject(nullptr),
	error(error)
{
}

void ErrorDialogGui::CreateGui()
{
	ImGui::OpenPopup("Error");

	if (ImGui::BeginPopupModal("Error"))
	{
		ImGui::Text("ERROR");
		ImGui::Text(error);

		if (ImGui::Button("Okay"))
		{
			close = true;
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
}