#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
	Transform() :
		m_position(),
		m_size(),
		m_rotate(0.0f)
	{}
	Transform(const glm::vec2& pos, const glm::vec2& size, const float rotate = 0.0f, float depth = 0.0f) :
		m_position(pos),
		m_size(size),
		m_rotate(rotate),
		m_depth(depth)
	{}

	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotate;

	float m_depth;
};