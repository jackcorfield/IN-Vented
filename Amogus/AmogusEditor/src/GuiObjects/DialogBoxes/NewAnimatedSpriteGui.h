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

	float inputInterval;
	glm::vec3 inputColour;

	struct TextureParams
	{
		TextureParams() :
			inputTextureName("New texture"),
			inputTextureFilePath("")
		{}

		char inputTextureName[512];
		char inputTextureFilePath[512];
	};

	// Texture data
	std::vector<TextureParams> m_inputTextureParams;
	int m_selectedTextureParamsIndex;

	// Shader data
	char inputShaderName[512];
	char inputVertexPath[512];
	char inputFragmentPath[512];
	char inputGeometryPath[512];
};