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

void PhysicsSystem::MovementPhysics(Physics* physics, Transform* transform, float deltaTime)
{
    physics->UpdateNetForce();
    physics->UpdateAcceleration();
    physics->UpdateVelocity();
    physics->ResetNetForce();

    CalculateMovement(physics, transform, deltaTime);
}

void PhysicsSystem::CalculateMovement(Physics* physics, Transform* transform, const float deltaTime)
{
    // update world position of object by adding displacement to previously calculated position
    physics->SetPosition(physics->addScaledVector(transform->m_position, physics->GetVelocity(), deltaTime));
    transform->m_position = physics->GetPosition();
}
