#include "CollisionManager.h"
#include <Core/source.h>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/BoxCollider.h>

#include <utility>

extern Application* g_app;

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

			std::pair<glm::vec2, glm::vec2> test = std::make_pair(*b2->m_position, b2->m_size);
			
			Entity us = activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b1);
			Physics* ourPhysics = activeScene->m_entityManager->GetComponent<Physics>(us);
			Transform* ourTransform = activeScene->m_entityManager->GetComponent<Transform>(us);

			if (b1->BoxCollision(test) && ourPhysics->m_velocity.length() != 0.0f)
			{
				float mtvDistance = FLT_MAX;
				glm::vec2 mtvAxis;

				if (!ResolveCollision(glm::vec2(1, 0), b1->m_position->x, b2->m_position->x, b1->m_position->x + (b1->m_size.x/2), b2->m_position->x + (b2->m_size.x / 2), mtvAxis, mtvDistance))
					return;

				if (!ResolveCollision(glm::vec2(0, 1), b1->m_position->y, b2->m_position->y, b1->m_position->y + (b1->m_size.y / 2), b2->m_position->y + (b2->m_size.y / 2), mtvAxis, mtvDistance))
					return;

				glm::vec2 penetrationVector = glm::normalize(mtvAxis);
				float penetrationDepth = sqrt(mtvDistance) * 1.001f;
				glm::vec2 resolutionVector = (penetrationVector * penetrationDepth);

				ourTransform->m_position += resolutionVector;
			}
		}
	}
}
