#pragma once

#include "../IGuiObject.h"

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

	glm::vec3 inputColour;

	// Texture data
	char inputTextureName[128];
	char inputTextureFilePath[128];
	glm::vec2 inputFrameSize;

	// Shader data
	char inputShaderName[128];
	char inputVertexPath[128];
	char inputFragmentPath[128];
	char inputGeometryPath[128];
};