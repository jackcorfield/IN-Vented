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
	if (m_seekTo == 0)
	{
		FindPlayer();
	}
}

void EnemyMovementScript::OnUpdate(float dt)
{
	// If there is no target entity, try to find one. If there is still no target, return
	if (m_seekTo == 0) { FindPlayer(); }
	if (m_seekTo == 0) { return; }

	// Calculate direction toward target entity
	Transform* seekToTransform = g_app->m_sceneManager->GetActiveScene()->m_entityManager->GetComponent<Transform>(m_seekTo);
	glm::vec2 dir = Seek(m_transform->m_position, seekToTransform->m_position);

	// Move toward target entity
	m_transform->m_position += dir * m_moveSpeed * dt;
}

void EnemyMovementScript::OnRender(float dt)
{}

void EnemyMovementScript::OnUnattach()
{}

glm::vec2 Seek(glm::vec2 pos, glm::vec2 seekTo)
{
	return glm::normalize(seekTo - pos);
}

Entity FindPlayer()
{
	Entity player = 0;

	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	auto allEntities = entityManager->GetAllActiveEntities();

	// Loop through all entities and check if their name is "Player". If it is, it is a suitable target entity
	for (Entity entity : allEntities)
	{
		EntityName* name = entityManager->GetComponent<EntityName>(entity);
		if (name && name->m_name == "Player")
		{
			player = entity;
			break;
		}
	}

	return player;
}