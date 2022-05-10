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

		char inputTextureName[128];
		char inputTextureFilePath[128];
	};

	// Texture data
	std::vector<TextureParams> m_inputTextureParams;
	int m_selectedTextureParamsIndex;

	// Shader data
	char inputShaderName[128];
	char inputVertexPath[128];
	char inputFragmentPath[128];
	char inputGeometryPath[128];
};