#pragma once

#include "Resources/Script.h"

#include "../Amogus.h"

class EnemyMovementScript :
    public Script
{
public:
	EnemyMovementScript(EntityManager* entityManager, Entity parentEntityID, float moveSpeed = 1.0f, Entity playerEntityID = 0);
	virtual void OnAttach() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender(float dt) override;
	virtual void OnUnattach() override;

private:
	float m_moveSpeed;
	Entity m_seekTo;

	Transform* m_transform;
};