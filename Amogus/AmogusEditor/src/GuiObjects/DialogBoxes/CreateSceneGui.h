#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct CreateSceneGui :
	public IGuiObject
{
public:
	CreateSceneGui();

	virtual void CreateGui() override;

private:
	char inputName[128];
};