#include "EditAnimationsGui.h"

#include <ImGui/imgui.h>

EditAnimationsGui::EditAnimationsGui(Entity entityFor) :
	IGuiObject(nullptr),
	selectedAnimation("##"),
	selectedFrameIndex(0),
	inputFrameNo(0),
	inputNewAnimationName(""),
	entity(entityFor)
{}

void EditAnimationsGui::CreateGui()
{
	ImGui::OpenPopup("Edit animations");

	if (ImGui::BeginPopupModal("Edit animations"))
	{
		AnimatedSprite* animatedSprite = g_app->m_sceneManager->GetActiveScene()->m_entityManager->GetComponent<AnimatedSprite>(entity);
		if (!animatedSprite) { return; }

		// Get all keys in animation map
		std::map<std::string, Animation>& allAnimations = animatedSprite->getAnimations();
		std::vector<std::string> allKeys;
		for (auto i = allAnimations.begin(); i != allAnimations.end(); i++)
		{
			allKeys.emplace_back(i->first);
		}

		// Add new animation
		if (ImGui::InputText("Name##", inputNewAnimationName, IM_ARRAYSIZE(inputNewAnimationName))) {}
		if (ImGui::Button("Add animation##"))
		{
			allAnimations.emplace(std::string(inputNewAnimationName), Animation());
		}

		// List keys for selection
		if (allKeys.size() <= 0)
		{
			selectedAnimation = "##";
		}
		if (ImGui::BeginCombo("Animations##", selectedAnimation.c_str()))
		{
			for (const std::string& key : allKeys)
			{
				bool isSelected = selectedAnimation == key;
				if (ImGui::Selectable(key.c_str(), isSelected))
				{
					selectedAnimation = key;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Separator();

		// Display info for selected animation
		if (selectedAnimation != "" && selectedAnimation != "##" && allAnimations.size() != 0)
		{
			Animation& animation = allAnimations[selectedAnimation];

			// Frame time/interval
			if (ImGui::DragFloat("Frame time##", &animation.frameTime)) {}

			// Frame selection
			std::string preview = "";
			if (animation.frames.size() > 0)
			{
				preview = std::to_string(selectedFrameIndex);
			}
			if (ImGui::BeginCombo("Frames##", preview.c_str()))
			{
				for (unsigned int i = 0; i < animation.frames.size(); i++)
				{
					bool isSelected = i == selectedFrameIndex;
					unsigned int frame = animation.frames[i];
					if (ImGui::Selectable(std::to_string(frame).c_str(), isSelected))
					{
						selectedFrameIndex = i;
					}
				}

				ImGui::EndCombo();
			}

			// Delete frame
			ImGui::SameLine();
			if (ImGui::Button("-"))
			{
				animation.frames.erase(animation.frames.begin() + selectedFrameIndex);
			}

			// Add new frame
			ImGui::Separator();
			ImGui::Text("Add frame");

			if (ImGui::InputInt("Frame no", &inputFrameNo))
			{
				if (inputFrameNo < 0)
				{
					inputFrameNo = 0;
				}
			}
			if (ImGui::Button("+"))
			{
				animation.frames.emplace_back(inputFrameNo);
			}
		}

		if (ImGui::Button("Finish##"))
		{
			ImGui::CloseCurrentPopup();
			close = true;
		}

		ImGui::EndPopup();
	}
}