#pragma once

struct IGuiObject
{
	IGuiObject()
		: close(false)
	{}
	virtual ~IGuiObject() {}

	virtual void CreateGui() = 0;

	bool close;
};