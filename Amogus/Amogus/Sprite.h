#pragma once
#include <glm.hpp>
#include "Shader.h"
#include "Texture2D.h"

struct SpriteVertex
{
	glm::vec2 pos;
	glm::vec2 texCoords;
};

class Sprite
{
public:
	Sprite(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture, const glm::vec3& colour = glm::vec3(1.0f), const float rotate = 0, Shader* shader = nullptr);

	inline glm::vec2 GetPosition() { return m_position; }
	inline glm::vec2 GetSize() { return m_size; }
	inline float GetRotate() { return m_rotate; }
	inline glm::vec3 GetColour() { return m_colour; }
	inline Shader* GetShader() { return m_shader; }
	inline Texture2D GetTexture() { return m_texture; }

private:
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotate;
	glm::vec3 m_colour;
	Shader* m_shader;
	Texture2D m_texture;
};

