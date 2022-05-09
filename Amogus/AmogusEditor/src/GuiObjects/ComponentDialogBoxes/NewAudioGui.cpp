#include "NewAudioGui.h"

#include <ImGui/imgui.h>

NewAudioGui::NewAudioGui(Entity entityFor) :
	entity(entityFor),
	inputFilePath("")
{}

void NewAudioGui::CreateGui()
{
	if (ImGui::Begin("New audio"))
	{
		if (ImGui::InputText("Audio filepath##", inputFilePath, IM_ARRAYSIZE(inputFilePath))) {}

		// Channel group combo box
		{
			std::string previewName;
			FMOD::ChannelGroup* groups[2] = { g_app->m_audioManager->m_sfx, g_app->m_audioManager->m_bgm };
			std::string names[2] = { "sfx", "bgm" };

			if (selectedChannelGroup == g_app->m_audioManager->m_sfx)
			{
				previewName = names[0];
			}
			else
			{
				previewName = names[1];
			}

			if (ImGui::BeginCombo("Channel group", previewName.c_str()))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = selectedChannelGroup == groups[i];

					if (ImGui::Selectable(names[i].c_str(), isSelected))
					{
						selectedChannelGroup = groups[i];
					}
				}

				ImGui::EndCombo();
			}
		}

		if (ImGui::Button("Add audio##"))
		{
			AddAudio();
			close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			close = true;
		}
	}

	ImGui::End();
}

void NewAudioGui::AddAudio()
{
	std::string filePath(inputFilePath);
	FMOD::System* system(g_app->m_audioManager->m_system);

	g_app->m_sceneManager->GetActiveScene()->m_entityManager->AddComponent<Audio>(entity, filePath.c_str(), system, selectedChannelGroup);
}