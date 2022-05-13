#include "EnemySpawnerScript.h"

// Tries to find an entity called "Player" to spawn enemies around
Entity FindPlayer();

EnemySpawnerScript::EnemySpawnerScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID) :
	Script(entityManager, parentEntityID),
	m_spawnAround(playerEntityID),
	m_totalTimeElapsed(0.0f),
	m_timeElapsedSinceLastWave(0.0f),
	m_enemiesAlive(0),
	m_maxEnemiesAlive(500),
	m_timeBetweenWaveSpawns(60.0f)
{}

void EnemySpawnerScript::OnAttach()
{
	// If no player ID was provided in constructor, try to find player to spawn enemies around
	if (m_spawnAround == 0) { m_spawnAround = FindPlayer(); }
	if (m_spawnAround == 0) { return; }


}

void EnemySpawnerScript::OnUpdate(float dt)
{
	m_totalTimeElapsed += dt;
	m_timeElapsedSinceLastWave += dt;

	if (m_timeElapsedSinceLastWave >= m_timeBetweenWaveSpawns)
	{
		SpawnWave();
		m_timeElapsedSinceLastWave = 0.0f;
	}

	// If there is no target entity, try to find one. If there is still no target, return
	if (m_spawnAround == 0) { m_spawnAround = FindPlayer(); }
	if (m_spawnAround == 0) { return; }
}

void EnemySpawnerScript::OnRender(float dt)
{}

void EnemySpawnerScript::OnUnattach()
{}

void EnemySpawnerScript::SpawnWave()
{
	for (int i = 0; i < 20; i++)
	{
		SpawnEnemy();
	}
}

void EnemySpawnerScript::SpawnEnemy()
{
	int maxRandom = m_templates.size() - 1;
	int selectedTemplate = rand() % maxRandom;

	Entity enemyTemplate = m_templates[selectedTemplate];
	if (m_enemiesAlive < m_maxEnemiesAlive)
	{
		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
		Entity newEntity = entityManager->CreateEntity();
		m_enemyEntities.emplace_back(newEntity);

		// Set up animated sprite
		entityManager->AddComponent<AnimatedSprite>(newEntity);

		// Set up name
		entityManager->AddComponent<EntityName>(newEntity, "Enemy");
		entityManager->AddComponent<ScriptComponent>(newEntity, entityManager, newEntity);
		entityManager->AddComponent<Transform>(newEntity);
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