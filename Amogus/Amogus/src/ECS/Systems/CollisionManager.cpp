#include "CollisionManager.h"
#include <Core/source.h>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/BoxCollider.h>

#include <utility>

extern Application* g_app;

bool BoxVBoxCollision(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2)
{
	glm::vec2 halfSize1 = size1 / 2.0f;
	glm::vec2 halfSize2 = size2 / 2.0f;

	glm::vec2 topLeftA = glm::vec2(pos1.x - halfSize1.x, pos1.y - halfSize1.y);
	glm::vec2 bottomRightA = glm::vec2(pos1.x + halfSize1.x, pos1.y + halfSize1.y);
	glm::vec2 topLeftB = glm::vec2(pos2.x - halfSize2.x, pos2.y - halfSize2.y);
	glm::vec2 bottomRightB = glm::vec2(pos2.x + halfSize2.x, pos2.y + halfSize2.y);

	if (topLeftA.x < bottomRightB.x &&
		bottomRightA.x > topLeftB.x &&
		topLeftA.y < bottomRightB.y &&
		bottomRightA.y > topLeftB.y)
		return true;

	return false;
}

bool CollisionManager::ResolveCollision(glm::vec2 axis, float min_a, float min_b, float max_a, float max_b, glm::vec2& mtvAxis, float& mtvDistance)
{
	float axisLengthSquared = glm::dot(axis, axis);

	float d0 = (max_b - min_a);
	float d1 = (max_a - min_b);

	if (d0 <= 0.0f || d1 <= 0.0)
		return false;

	float overlap = (d0 < d1) ? d0 : -d1;

	glm::vec2 sep = axis * (overlap / axisLengthSquared);
	float sepLengthSquared = glm::dot(sep, sep);

	if (sepLengthSquared < mtvDistance)
	{
		mtvDistance = sepLengthSquared;
		mtvAxis = sep;
	}

	return true;
}

void CollisionManager::CheckCollision()
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
	if (!activeScene) return;

	std::vector<BoxCollider*> boxColliders = activeScene->m_entityManager->GetAllComponentsOfType<BoxCollider>();

	for (BoxCollider* b1 : boxColliders)
	{
		for(BoxCollider* b2 : boxColliders)
		{
			if (b1 == b2) continue;

			if(activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b2) == activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b1))
				continue;
			
			Entity us = activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b1);
			Entity them = activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b2);

			Physics* ourPhysics = activeScene->m_entityManager->GetComponent<Physics>(us);
			Transform* t1 = activeScene->m_entityManager->GetComponent<Transform>(us);
			Transform* t2 = activeScene->m_entityManager->GetComponent<Transform>(them);

			if (BoxVBoxCollision(t1->m_position, b1->m_size, t2->m_position, b2->m_size) && ourPhysics->m_velocity.length() != 0.0f)
			{
				float mtvDistance = FLT_MAX;
				glm::vec2 mtvAxis;

				if (!ResolveCollision(glm::vec2(1, 0), t1->m_position.x, t1->m_position.x, t1->m_position.x + (b1->m_size.x / 2), t2->m_position.x + (b2->m_size.x / 2), mtvAxis, mtvDistance))
					return;

				if (!ResolveCollision(glm::vec2(0, 1), t1->m_position.y, t1->m_position.y, t1->m_position.y + (b1->m_size.y / 2), t2->m_position.y + (b2->m_size.y / 2), mtvAxis, mtvDistance))
					return;

				glm::vec2 penetrationVector = glm::normalize(mtvAxis);
				float penetrationDepth = sqrt(mtvDistance) * 1.001f;
				glm::vec2 resolutionVector = (penetrationVector * penetrationDepth);

				t1->m_position += resolutionVector;
			}
		}
	}
}
