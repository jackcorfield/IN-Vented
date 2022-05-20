#include "EnemyMovementScript.h"

// Returns direction vector toward player
glm::vec2 Seek(glm::vec2 pos, glm::vec2 seekTo);

// Tries to find an entity called "Player" to seek towards
Entity FindPlayer();

EnemyMovementScript::EnemyMovementScript(EntityManager* entityManager, Entity parentEntityID, float moveSpeed, Entity playerEntityID) :
	Script(entityManager, parentEntityID),
	m_moveSpeed(moveSpeed),
	m_seekTo(playerEntityID),
	m_transform(nullptr)
{}

void EnemyMovementScript::OnAttach()
{
	// Store this entity's transform so we don't need to retrieve it every frame.
	// We can do this because we can reasonably assume that the transform will exist while this script does
	m_transform = GetComponent<Transform>();

	// If no player ID was provided in constructor, try to find player to seek to
	if (m_seekTo == 0) { m_seekTo = FindPlayer(); }
}

void EnemyMovementScript::OnUpdate(float dt)
{
	// If there is no target entity, try to find one. If there is still no target, return
	if (m_seekTo == 0) { m_seekTo = FindPlayer(); }
	if (m_seekTo == 0) { return; }

	// Calculate direction toward target entity
	Transform* seekToTransform = g_app->m_sceneManager->GetActiveScene()->m_entityManager->GetComponent<Transform>(m_seekTo);
	glm::vec2 dir = Seek(m_transform->m_position, seekToTransform->m_position); // Think about maybe only creating a new dir a few times per second, rather than every frame. Done right, this could be a big optimisation

	// Move toward target entity
	m_transform->m_position += dir * m_moveSpeed * dt;

	CheckCollisions();
}

void EnemyMovementScript::OnRender(float dt)
{}

void EnemyMovementScript::OnUnattach()
{}

glm::vec2 Seek(glm::vec2 pos, glm::vec2 seekTo)
{
	return glm::normalize(seekTo - pos);
}

void EnemyMovementScript::CheckCollisions()
{
	std::vector<Entity> allPossibleCollisions = GetNearbyEntities();
	float size = allPossibleCollisions.size();
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	for (Entity possibleCollision : allPossibleCollisions)
	{
		// Check that this is an enemy
		EntityName* name = entityManager->GetComponent<EntityName>(possibleCollision);
		if (name->m_name != "Enemy") { continue; }

		// Get relevant components
		BoxCollider* myCollider = GetComponent<BoxCollider>();
		Transform* myTransform = GetComponent<Transform>();

		BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(possibleCollision);
		Transform* theirTransform = entityManager->GetComponent<Transform>(possibleCollision);

		// Calculate these for convenience
		float myTop = myTransform->m_position.y;
		float myBottom = myTransform->m_position.y + myCollider->m_size.y;
		float myLeft = myTransform->m_position.x;
		float myRight = myTransform->m_position.x + myCollider->m_size.x;

		float theirTop = theirTransform->m_position.y;
		float theirBottom = theirTransform->m_position.y + myCollider->m_size.y;
		float theirLeft = theirTransform->m_position.x;
		float theirRight = theirTransform->m_position.x + myCollider->m_size.x;

		// Quick check for collision
		if (myRight < theirLeft || myLeft > theirRight || myBottom < theirTop || myTop > theirBottom)
		{
			continue; // No collision
		}

		// Now that we know there is a collision, calculate depth so we can resolve it
		float depthX = myRight - theirLeft;
		float depthY = myBottom - theirTop;

		m_transform->m_position.x -= depthX;
		m_transform->m_position.y -= depthY;
	}
}

Entity FindPlayer()
{
	Entity player = 0;

	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	auto allNameComponents = entityManager->GetAllComponentsOfType<EntityName>();
	for (EntityName* nameComponent : allNameComponents)
	{
		if (nameComponent->m_name == "Player")
		{
			player = entityManager->GetEntityFromComponent<EntityName>(nameComponent);
		}
	}

	return player;
}