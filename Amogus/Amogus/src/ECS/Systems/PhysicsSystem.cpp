#include "PhysicsSystem.h"

#include <Core/source.h>

extern Application* g_app;

void PhysicsSystem::Update(float deltaTime)
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
	if (!activeScene) return;

    std::vector<Physics*> physicsComponents = activeScene->m_entityManager->GetAllComponentsOfType<Physics>();

    for (Physics* p : physicsComponents)
    {
		// Get our components and stuff
		Entity physicsEntity = activeScene->m_entityManager->GetEntityFromComponent<Physics>(p);
		Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(physicsEntity);

		if (physicsEntity == NULL || transform == nullptr)
			continue;

		if (p == nullptr)
			continue;

		// Update to get our velocity
		p->Update(deltaTime);

		// Apply our velocity made into displacement
		transform->m_position += p->m_velocity * deltaTime;
    }
}