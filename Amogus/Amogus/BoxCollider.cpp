#include "BoxCollider.h"
BoxCollider::BoxCollider(glm::vec2& position, glm::vec2 size) :
	m_position(&position),
	m_size(size)
{

}

bool BoxCollider::BoxCollision(std::pair<glm::vec2, glm::vec2> target) // AABB - AABB collision
{
	glm::vec2 topLeftA = glm::vec2(m_position->x - m_size.x, m_position->y - m_size.y);
	glm::vec2 bottomRightA = glm::vec2(m_position->x + m_size.x, m_position->y + m_size.y);
	glm::vec2 topLeftB = glm::vec2(target.first.x - target.second.x, target.first.y - target.second.y);
	glm::vec2 bottomRightB = glm::vec2(target.first.x + target.second.x, target.first.y + target.second.y);

	if (topLeftA.x < bottomRightB.x &&
		bottomRightA.x > topLeftB.x &&
		topLeftA.y < bottomRightB.y &&
		bottomRightA.y > topLeftB.y)
		return true;

	return false;
}