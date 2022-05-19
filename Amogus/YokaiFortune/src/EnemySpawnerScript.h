#pragma once

#include "Resources/Script.h"

#include "../Amogus.h"

class EnemySpawnerScript :
	public Script
{
public:
	EnemySpawnerScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID = 0);

	virtual void OnAttach() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender(float dt) override;
	virtual void OnUnattach() override;

private:
	void SpawnWave();
	void SpawnEnemy();

	void CloneEnemy(Entity templateEntity, Entity targetEntity);

	// Tries to find an entity called "Player" to spawn enemies around
	Entity FindPlayer();

	Entity m_spawnAround; // Player entity that enemies spawn around

	float m_totalTimeElapsed; // Total time elapsed since game start
	float m_timeElapsedSinceLastWave; // Time elapsed since last wave of enemies was spawned
	unsigned int m_enemiesAlive; // The number of enemies currently alive.

	const unsigned int m_maxEnemiesAlive; // Hard cap on enemies alive at one time. Once this is reached, new enemies will despawn old enemies
	const float m_timeBetweenWaveSpawns; // Time that needs to elapse before another wave of enemies is spawned

	std::vector<Entity> m_enemyEntities; // All enemy entities. Stored so that they can be reused upon new enemy spawn
	std::vector<Entity> m_templates; // Template entities used to create new enemies
};