#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct ImportSceneGui :
	public IGuiObject
{
public:
	ImportSceneGui(ImGuiLayer* layer, std::list<std::string>* recentScenes);

	virtual void CreateGui() override;

private:
	char m_inputName[128];
	std::list<std::string>* p_recentScenes;
};