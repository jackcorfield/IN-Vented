#pragma once
#include "Physics.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	void PhysicsUpdate(float deltaTime);
	void MovementPhysics(Physics* p, Transform* t, float deltaTime);
	void CalculateMovement(Physics* p, Transform* transform, const float deltaTime);
};

