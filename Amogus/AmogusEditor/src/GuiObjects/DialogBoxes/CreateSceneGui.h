#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct CreateSceneGui :
	public IGuiObject
{
public:
	CreateSceneGui(ImGuiLayer* layer);

	virtual void CreateGui() override;

private:
	char m_inputName[512];
	float col[3];
};