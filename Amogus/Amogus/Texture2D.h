#pragma once

#include <string>

struct Texture2D
{
public:
	Texture2D(const unsigned int id, const unsigned int width, const unsigned int height, const std::string& name, const std::string& filePath);

	void Bind() const;
	void Unbind() const;

	const unsigned int m_id;
	const unsigned int m_width, m_height;
	const std::string m_name, m_filePath;
};

namespace TextureLoader
{
	Texture2D CreateTexture2DFromFile(const std::string& name, const std::string& filePath);
}