#include "NewAnimatedSpriteGui.h"

#include <ImGui/imgui.h>

NewAnimatedSpriteGui::NewAnimatedSpriteGui(Entity entityFor) :
	entity(entityFor)
{}

void NewAnimatedSpriteGui::CreateGui()
{


	if (ImGui::Button("Add animated sprite##"))
	{
		AddAnimatedSprite();
		close = true;
	}
}

void NewAnimatedSpriteGui::AddAnimatedSprite()
{

}