#pragma once
#include <glm.hpp>
#include "Shader.h"
#include "Texture2D.h"

class Sprite
{
public:
	Sprite(const Texture2D& texture, const glm::vec3& colour = glm::vec3(1.0f), Shader* shader = nullptr);

	inline glm::vec3 GetColour() const { return m_colour; }
	inline Shader* GetShader() const { return m_shader; }
	inline Texture2D GetTexture() const { return m_texture; }

private:
	glm::vec3 m_colour;
	Shader* m_shader;
	Texture2D m_texture;
};