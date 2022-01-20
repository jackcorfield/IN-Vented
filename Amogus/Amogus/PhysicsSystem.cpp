#include "PhysicsSystem.h"
#include "source.h"

extern Application* g_app;

PhysicsSystem::PhysicsSystem()
{


}

void PhysicsSystem::PhysicsUpdate(float deltaTime)
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    std::vector<Physics*> physicsComponents = activeScene->m_entityManager->GetAllComponentsOfType<Physics>();

    for (Physics* p : physicsComponents)
    {
        Entity physicsEntity = activeScene->m_entityManager->GetEntityFromComponent<Physics>(p);
        Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(physicsEntity);
        MovementPhysics(p, transform, deltaTime);
    }	
}

void PhysicsSystem::MovementPhysics(Physics* physics, Transform* t, float deltaTime)
{
    physics->UpdateAcceleration();
    physics->UpdateVelocity();
    //REPLACE WITH DELTA TIME PLS
    CalculateMovement(p, t, deltaTime);
}

void PhysicsSystem::CalculateMovement(Physics* physics, Transform* transform, const float deltaTime)
{
    // update world position of object by adding displacement to previously calculated position
    physics->SetPosition(transform->m_position);
    physics->SetPosition(physics->addScaledVector(physics->GetPosition(), physics->GetVelocity(), deltaTime));


    transform->m_position = physics->GetPosition();
}
