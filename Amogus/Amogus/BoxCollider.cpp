#include "BoxCollider.h"
BoxCollider::BoxCollider(glm::vec2& position, glm::vec2 size) :
	m_position(&position),
	m_size(size)
{

}

bool BoxCollider::BoxCollision(std::pair<glm::vec2, glm::vec2> target) // AABB - AABB collision
{
    bool collisionX = m_position->x + m_size.x >= target.first.x &&
        target.first.x + target.second.x >= m_position->x;

    bool collisionY = m_position->y + m_size.y >= target.first.y &&
        target.first.y + target.second.y >= m_position->y;

    return collisionX && collisionY;
}