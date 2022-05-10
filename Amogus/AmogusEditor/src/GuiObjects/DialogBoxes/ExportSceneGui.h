#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct ExportSceneGui :
	public IGuiObject
{
public:
	ExportSceneGui();

	virtual void CreateGui() override;

private:
	char inputFilePath[128];
};