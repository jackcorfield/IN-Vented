#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct NewSpriteGui :
	public IGuiObject
{
public:
	NewSpriteGui(Entity entityFor);

	virtual void CreateGui() override;

private:
	void AddSprite();

	Entity entity;

	// Texture data
	char inputTextureName[512];
	char inputTextureFilePath[512];
	glm::vec3 colour;

	// Shader data
	char inputShaderName[512];
	char inputVertexPath[512];
	char inputFragmentPath[512];
	char inputGeometryPath[512];
};