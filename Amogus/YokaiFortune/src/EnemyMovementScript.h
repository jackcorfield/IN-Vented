#pragma once

#include "Resources/Script.h"
#include "PlayerScript.h"
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

	void SetDirection(const glm::vec2& direction);
	bool m_isDead;

private:
	void UpdateSpriteAnimation(bool facingLeft);

	void CheckCollisions();
	bool CheckPotentialCollision(Entity possibleCollision); // Simple AABB collision check
	glm::vec2 GetIntersectionDepth(Entity collidedEntity);
	void ResolveCollision(glm::vec2 intersection, BoxCollider* theirCollider, Transform* theirTransform);

	glm::vec2 m_moveDir;
	float m_moveSpeed;
	bool m_facingLeft; // Used to determine whether to flip sprite

	Transform* m_transform;
	BoxCollider* m_collider;

	PlayerScript* m_pScript;
};