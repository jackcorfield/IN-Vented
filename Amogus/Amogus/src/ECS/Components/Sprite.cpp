#include "Sprite.h"

Sprite::Sprite(const Texture2D& texture, const glm::vec3& colour, Shader* shader) :
	m_texture(texture),
	m_colour(colour),
	m_shader(shader)
{}