#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct ImportSceneGui :
	public IGuiObject
{
public:
	ImportSceneGui();

	virtual void CreateGui() override;

private:
	char inputFilePath[128];
};