#pragma once

#include <string>

struct Texture2D
{
public:
	Texture2D(const unsigned int id, const unsigned int width, const unsigned int height);

	void Bind() const;
	void Unbind() const;

	unsigned int m_id;
	unsigned int m_width, m_height;
};

namespace TextureLoader
{
	Texture2D CreateTexture2DFromFile(const std::string& name, const std::string& filePath);
}