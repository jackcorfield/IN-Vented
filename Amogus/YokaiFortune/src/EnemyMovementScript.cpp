#include "EnemyMovementScript.h"

#define MAX_Y 180.0f
#define MIN_Y -180.0f
#define PADDING 50.0f // To prevent sudden disappearance at edges (due to depth >= 1.0f)

EnemyMovementScript::EnemyMovementScript(EntityManager* entityManager, Entity parentEntityID, float moveSpeed, Entity playerEntityID) :
	Script(entityManager, parentEntityID),
	m_isDead(false),
	m_moveDir(0.0f),
	m_moveSpeed(moveSpeed),
	m_facingLeft(false),
	m_transform(nullptr),
	m_collider(nullptr)
{}

void EnemyMovementScript::OnAttach()
{
	// Store this entity's transform and collider so we don't need to retrieve it every frame.
	// We can do this because we can reasonably assume that these will exist while this script does
	m_transform = GetComponent<Transform>();
	m_collider = GetComponent<BoxCollider>();
}

void EnemyMovementScript::OnUpdate(float dt)
{
	if (m_isDead) { return; }
	// Move in set direction
	m_transform->m_position += m_moveDir * m_moveSpeed * dt;

	float min = MIN_Y - PADDING;
	float max = MAX_Y + PADDING;
	m_transform->m_depth = -((m_transform->m_position.y - min) / (max - min)); // Depth is negative of percentage between 0 and 1 of y compared to screen height

	// Test nearby entities for collision and resolve any that have occurred
	CheckCollisions();
}

void EnemyMovementScript::OnRender(float dt)
{}

void EnemyMovementScript::OnUnattach()
{}

void EnemyMovementScript::SetDirection(const glm::vec2& direction)
{
	m_moveDir = direction;

	// Update sprite direction based on move direction
	bool facingLeft = m_moveDir.x < 0;
	UpdateSpriteAnimation(facingLeft);
}

void EnemyMovementScript::UpdateSpriteAnimation(bool facingLeft)
{
	bool changedDirection = facingLeft != m_facingLeft;
	if (changedDirection) // If changed direction, reverse size
	{
		m_transform->m_size.x = -m_transform->m_size.x;
	}

	// Update member based on current frame
	m_facingLeft = facingLeft;
}

void EnemyMovementScript::CheckCollisions()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	std::vector<Entity> allPossibleCollisions = GetNearbyEntities();
	for (Entity possibleCollision : allPossibleCollisions)
	{
		bool intersecting = CheckPotentialCollision(possibleCollision);
		if (!intersecting) { continue; }

		glm::vec2 intersection(GetIntersectionDepth(possibleCollision));

		BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(possibleCollision);
		Transform* theirTransform = entityManager->GetComponent<Transform>(possibleCollision);
		ResolveCollision(intersection, theirCollider, theirTransform);
	}
}

bool EnemyMovementScript::CheckPotentialCollision(Entity possibleCollision)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Check that this is an enemy
	EntityName* name = entityManager->GetComponent<EntityName>(possibleCollision);
	if (!name)
		return false;
	if (name->m_name != "Enemy" && name->m_name != "Player") { return false; }

	// Get relevant components
	BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(possibleCollision);
	Transform* theirTransform = entityManager->GetComponent<Transform>(possibleCollision);

	// Calculate these for convenience
	glm::vec2 myHalfSize = m_collider->m_size / 2.0f;
	float myTop = m_transform->m_position.y - myHalfSize.y + m_collider->m_offset.y;
	float myBottom = m_transform->m_position.y + myHalfSize.y + m_collider->m_offset.y;
	float myLeft = m_transform->m_position.x - myHalfSize.x + m_collider->m_offset.x;
	float myRight = m_transform->m_position.x + myHalfSize.x + m_collider->m_offset.x;

	glm::vec2 theirHalfSize = theirCollider->m_size / 2.0f;
	float theirTop = theirTransform->m_position.y - theirHalfSize.y + theirCollider->m_offset.y;
	float theirBottom = theirTransform->m_position.y + theirHalfSize.y + theirCollider->m_offset.y;
	float theirLeft = theirTransform->m_position.x - theirHalfSize.x + theirCollider->m_offset.x;
	float theirRight = theirTransform->m_position.x + theirHalfSize.x + theirCollider->m_offset.x;

	// Quick check for collision
	if (myRight < theirLeft || myLeft > theirRight || myBottom < theirTop || myTop > theirBottom)
	{
		return false; // No collision
	}

	return true;
}

glm::vec2 EnemyMovementScript::GetIntersectionDepth(Entity collidedEntity)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Get relevant components
	BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(collidedEntity);
	Transform* theirTransform = entityManager->GetComponent<Transform>(collidedEntity);

	glm::vec2 myPos = m_transform->m_position + m_collider->m_offset;
	glm::vec2 theirPos = theirTransform->m_position + theirCollider->m_offset;

	float wy = (m_collider->m_size.x + theirCollider->m_size.x) * (myPos.y - theirPos.y);
	float hx = (m_collider->m_size.y + theirCollider->m_size.y) * (myPos.x - theirPos.x);

	glm::vec2 depth;
	if (wy > hx)
	{
		if (wy > -hx) // Top
		{
			depth.y = -1;
		}
		else // Left
		{
			depth.x = 1;
		}
	}
	else
	{
		if (wy > -hx) // Right
		{
			depth.x = -1;
		}
		else // Bottom
		{
			depth.y = 1;
		}
	}
		
	return depth;
}

void EnemyMovementScript::ResolveCollision(glm::vec2 intersection, BoxCollider* theirCollider, Transform* theirTransform)
{
	glm::vec2 theirHalfSize = theirCollider->m_size / 2.0f;

	float theirTop = theirTransform->m_position.y - theirHalfSize.y + theirCollider->m_offset.y;
	float theirBottom = theirTransform->m_position.y + theirHalfSize.y + theirCollider->m_offset.y;
	float theirLeft = theirTransform->m_position.x - theirHalfSize.x + theirCollider->m_offset.x;
	float theirRight = theirTransform->m_position.x + theirHalfSize.x + theirCollider->m_offset.x;

	glm::vec2 myHalfSize = GetComponent<BoxCollider>()->m_size / 2.0f;

	if (intersection.y == 1) // Bottom
	{
		m_transform->m_position.y = theirTop - myHalfSize.y - m_collider->m_offset.y;
	}
	else if (intersection.y == -1) // Top
	{
		m_transform->m_position.y = theirBottom + myHalfSize.y - m_collider->m_offset.y;
	}

	if (intersection.x == 1) // Left
	{
		m_transform->m_position.x = theirLeft - myHalfSize.x - m_collider->m_offset.x;
	}
	else if (intersection.x == -1) // Right
	{
		m_transform->m_position.x = theirRight + myHalfSize.x - m_collider->m_offset.x;
	}
}