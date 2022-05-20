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
	void CheckCollisions();
	bool CheckPotentialCollision(Entity possibleCollision); // Simple AABB collision check
	glm::vec2 GetIntersectionDepth(Entity collidedEntity);
	void ResolveCollision(glm::vec2 intersection, BoxCollider* theirCollider, Transform* theirTransform);

	float m_moveSpeed;
	Entity m_seekTo;

	Transform* m_transform;
	BoxCollider* m_collider;
};