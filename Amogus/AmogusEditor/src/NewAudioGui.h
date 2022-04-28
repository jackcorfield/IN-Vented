#pragma once

#include "IGuiObject.h"

#include "Amogus.h"

struct NewAudioGui :
	public IGuiObject
{
public:
	NewAudioGui(Entity entityFor);

	virtual void CreateGui() override;

private:
	void AddAudio();

	Entity entity;

	char inputFilePath[128];
	FMOD::ChannelGroup* selectedChannelGroup;
};