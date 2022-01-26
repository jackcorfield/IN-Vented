#pragma once
#include "glm.hpp"
#include <utility>

class BoxCollider
{
public:
	//put in transform pos and size
	BoxCollider(glm::vec2& position, glm::vec2 size);
	bool BoxCollision(std::pair<glm::vec2, glm::vec2> target);

	glm::vec2* m_position;
	glm::vec2 m_size;

private:

};

