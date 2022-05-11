#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct LoadGameGui :
	public IGuiObject
{
public:
	LoadGameGui(ImGuiLayer* layer);

	virtual void CreateGui() override;

private:
	char m_inputName[128];
};