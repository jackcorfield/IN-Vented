#include "SettingsGui.h"

#include <experimental/filesystem>
#include <fstream>
#include <ImGui/imgui.h>
#include <iostream>
#include <string>

#include "../../ImguiLayer.h"

namespace filesystem = std::experimental::filesystem;

SettingsGui::SettingsGui(ImGuiLayer* layer) :
	IGuiObject(layer)
{}

void SettingsGui::CreateGui()
{
	ImGui::OpenPopup("Accessiblity");

	if (ImGui::BeginPopupModal("Accessiblity"))
	{
		//FontSelect();
		ConsoleTextColourSelect();

		if (ImGui::Checkbox("Rounded corners", &layer->m_roundedCorners))
		{
			layer->ChangeStyle();
		}

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			close = true;
		}

		ImGui::EndPopup();
	}
}

void SettingsGui::FontSelect()
{
	// Skim fonts folder and retrieve all fonts
	std::vector<std::string> fontPaths;
	{
		std::string path = "Fonts";

		for (const auto& entry : filesystem::directory_iterator(path))
		{
			std::string filePath = entry.path().u8string();

			std::string ttf = ".ttf";
			std::string otf = ".otf";

			if (filePath.find(ttf) != std::string::npos) // If file is a .ttf file
			{
				fontPaths.emplace_back(filePath);
			}
			else if (filePath.find(otf) != std::string::npos) // If file is a .otf file
			{
				fontPaths.emplace_back(filePath);
			}
		}
	}

	// Create combo box to list fonts
	std::string selected = layer->GetFontPath();

	if (ImGui::BeginCombo("Text font", selected.c_str()))
	{
		for (const std::string& fontPath : fontPaths)
		{
			bool isSelected = selected == fontPath;
			if (ImGui::Selectable(fontPath.c_str(), isSelected))
			{
				selected = fontPath;
			}
		}

		ImGui::EndCombo();
	}
	if (selected != layer->GetFontPath())
	{
		layer->ChangeFont(selected, layer->GetFontSize());
	}
}

void SettingsGui::FontSizeSelect()
{

}

void SettingsGui::ConsoleTextColourSelect()
{
	bool changed = false;

	// Debug colour
	ImVec4 debugColour(layer->GetDebugColour());
	if (ImGui::ColorEdit4("Debug colour", &debugColour.x))
	{
		changed = true;
	}

	// Warning colour
	ImVec4 warningColour(layer->GetWarningColour());
	if (ImGui::ColorEdit4("Warning colour", &warningColour.x))
	{
		changed = true;
	}

	// Error colour
	ImVec4 errorColour(layer->GetErrorColour());
	if (ImGui::ColorEdit4("Error colour", &errorColour.x))
	{
		changed = true;
	}

	if (changed)
	{
		layer->SetConsoleColours(debugColour, warningColour, errorColour);
	}
}