#pragma once
#include "Physics.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	void PhysicsUpdate(float deltaTime);
	void MovementPhysics(Physics* physics, Transform* transform, float deltaTime);
	void CalculateMovement(Physics* physics, Transform* transform, const float deltaTime);
};

