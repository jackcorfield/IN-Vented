#include "PhysicsSystem.h"
#include "source.h"

extern Application* g_app;

PhysicsSystem::PhysicsSystem()
{


}

void PhysicsSystem::DoPhysics()
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    std::vector<Physics*> physicsComponents = activeScene->m_entityManager->GetAllComponentsOfType<Physics>();

    for (Physics* p : physicsComponents)
    {
        Entity physicsEntity = activeScene->m_entityManager->GetEntityFromComponent<Physics>(p);
        Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(physicsEntity);

        p->UpdateAcceleration();
        p->UpdateVelocity();
        //REPLACE WITH DELTA TIME PLS
        p->CalculateMovement(transform, 0.6);
    }
	
}