#pragma once

#include "IGuiObject.h"

#include "Amogus.h"

struct NewAnimatedSpriteGui :
	public IGuiObject
{
public:
	NewAnimatedSpriteGui(Entity entityFor);

	virtual void CreateGui() override;

private:
	void AddAnimatedSprite();

	Entity entity;

};