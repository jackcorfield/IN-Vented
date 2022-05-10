#include "Texture2D.h"

#include <glad/glad.h>

#include <Handlers/FileHandler.h>

Texture2D::Texture2D() :
	m_id(0),
	m_width(0),
	m_height(0),
	m_name(""),
	m_filePath("")
{}

Texture2D::Texture2D(const unsigned int id, const unsigned int width, const unsigned int height, const std::string& name, const std::string& filePath) :
	m_id(id),
	m_width(width),
	m_height(height),
	m_name(name),
	m_filePath(filePath)
{}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

namespace TextureLoader
{
	Texture2D CreateTexture2DFromFile(const std::string& name, const std::string& filePath)
	{
		std::string f = "Assets/Sprites/" + filePath;
		PNG_Data data = FileHandler::ReadPNG(f.c_str());
		GLuint id;
		glGenTextures(1, &id);

		Texture2D texture(id, data.m_width, data.m_height, name, f);

		texture.Bind();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.m_width, data.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.m_image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		texture.Unbind();

		return texture;
	}
}