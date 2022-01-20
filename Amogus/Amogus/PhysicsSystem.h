#pragma once
#include "Physics.h"

class PhysicsSystem
{
public:
	PhysicsSystem();
	void DoPhysics(float deltaTime);
	void MovementPhysics(Physics* p, Transform* t, float deltaTime);
};

