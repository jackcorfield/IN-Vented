#include "Sprite.h"

Sprite::Sprite(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture, const glm::vec3& colour, const float rotate, Shader* shader):
m_position(position),
m_size(size),
m_texture(texture),
m_colour(colour),
m_rotate(rotate),
m_shader(shader)
{}