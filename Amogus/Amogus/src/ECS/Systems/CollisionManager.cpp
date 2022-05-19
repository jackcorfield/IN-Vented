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

CollisionManager::CollisionManager()
{
	m_spatialHash = new SpatialHash(200);
}

CollisionManager::~CollisionManager()
{
	delete m_spatialHash;
	m_spatialHash = nullptr;
}

void CollisionManager::update()
{
	m_spatialHash->updateAll();
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	std::vector<Transform*> transforms = ecs->GetAllComponentsOfType<Transform>();
	
	for (Transform* t : transforms)
	{
		Entity e = ecs->GetEntityFromComponent<Transform>(t);
		BoxCollider* b = ecs->GetComponent<BoxCollider>(e);
		
		if (b != NULL)
		{
			if (!m_spatialHash->isInHash(e))
			{
				m_spatialHash->insert(e);
			}
		}
	}
}

bool CollisionManager::checkCollision(Entity a, Entity b)
{
	std::vector<Entity> potentialCollisions = m_spatialHash->broadCollisionCheck(a);
	if (std::find(potentialCollisions.begin(), potentialCollisions.end(), b) == potentialCollisions.end())
		return false;

	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	Transform* t1 = ecs->GetComponent<Transform>(a);
	Transform* t2 = ecs->GetComponent<Transform>(b);
	BoxCollider* b1 = ecs->GetComponent<BoxCollider>(a);
	BoxCollider* b2 = ecs->GetComponent<BoxCollider>(b);

	if (t1 == NULL || t2 == NULL ||
		b1 == NULL || b2 == NULL)
		return false;
	
	return BoxVBoxCollision(t1->m_position, b1->m_size, t2->m_position, b2->m_size);
}

std::vector<Entity> CollisionManager::potentialCollisions(Entity e)
{
	return m_spatialHash->broadCollisionCheck(e);
}
