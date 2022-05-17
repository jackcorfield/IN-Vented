#pragma once

class ImGuiLayer;
struct IGuiObject
{
	IGuiObject(ImGuiLayer* layer)
		: close(false), layer(layer)
	{}
	virtual ~IGuiObject() {}

	virtual void CreateGui() = 0;

	bool close;
	ImGuiLayer* layer;
};