#pragma once
#include <Resources/Texture2D.h>
#include <glm/glm.hpp>

class UI_Image
{
public:
	UI_Image() : m_texture(TextureLoader::CreateTexture2DFromFile("UI_test", "hi.png")), m_position(0, 0, 0, 0), m_size(0, 100, 0, 100), m_zIndex(1) {}
	UI_Image(const Texture2D& texture, glm::vec4 pos, glm::vec4 size, int zIdx) : m_texture(texture), m_position(pos), m_size(size), m_zIndex(zIdx) {}

	Texture2D m_texture;
	int m_zIndex;
	// FORMAT: RELATIVE X, ABSOLUTE X, RELATIVE Y, ABSOLUTE Y
	glm::vec4 m_position;
	glm::vec4 m_size;
};