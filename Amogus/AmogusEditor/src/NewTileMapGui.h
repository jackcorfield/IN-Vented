#pragma once

#include "IGuiObject.h"

#include "Amogus.h"

struct NewTileMapGui :
	public IGuiObject
{
public:
	NewTileMapGui(Entity entityFor);

	virtual void CreateGui() override;

private:
	void AddTileMap();

	Entity entity;

	glm::vec2 inputTileSize;
	int inputMapWidth, inputMapHeight;
};