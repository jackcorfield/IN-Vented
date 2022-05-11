#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct NewGameGui :
	public IGuiObject
{
public:
	NewGameGui(ImGuiLayer* layer);

	virtual void CreateGui() override;

private:
	char m_inputName[128];
};