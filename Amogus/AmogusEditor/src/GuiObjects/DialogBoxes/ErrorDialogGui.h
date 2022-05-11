#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct ErrorDialogGui :
	public IGuiObject
{
public:
	ErrorDialogGui(const char* error);

	virtual void CreateGui() override;
	
private:
	const char* error;
};
