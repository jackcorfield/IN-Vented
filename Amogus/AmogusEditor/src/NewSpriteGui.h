#pragma once

#include "IGuiObject.h"

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
	char inputTextureName[128];
	char inputTextureFilePath[128];
	glm::vec3 colour;

	// Shader data
	char inputShaderName[128];
	char inputVertexPath[128];
	char inputFragmentPath[128];
	char inputGeometryPath[128];
};