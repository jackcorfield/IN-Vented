#include "CollisionManager.h"
#include "source.h"
#include "TileMap.h"
#include "BoxCollider.h"

#include <utility>

extern Application* g_app;

void CollisionManager::CheckCollision()
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
	if (!activeScene) return;

	std::vector<BoxCollider*> boxColliders = activeScene->m_entityManager->GetAllComponentsOfType<BoxCollider>();

	for (BoxCollider* b1 : boxColliders)
	{
		for(BoxCollider* b2 : boxColliders)
		{
			if(activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b2) == activeScene->m_entityManager->GetEntityFromComponent<BoxCollider>(b1))
				continue;

			std::pair<glm::vec2, glm::vec2> test = std::make_pair(*b2->m_position, b2->m_size);
			
			if(b1->BoxCollision(test))
				std::cout << "collision detection babyyyy" << std::endl;
		}
	}
}
