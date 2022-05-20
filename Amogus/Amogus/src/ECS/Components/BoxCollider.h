#pragma once
#include <glm/glm.hpp>
#include <utility>

class BoxCollider
{
public:
	BoxCollider(const glm::vec2& size, const glm::vec2& offset);

	glm::vec2 m_size;
	glm::vec2 m_offset;
};