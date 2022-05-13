#include "EnemySpawnerScript.h"

#include "EnemyMovementScript.h"

glm::vec2 SetRandomSpawnPos(const glm::vec2& viewport, const glm::vec2& centrePos);

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

	// Fill template vector
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	auto allNames = entityManager->GetAllComponentsOfType<EntityName>();
	for (EntityName* name : allNames)
	{
		if (name->m_name == "Enemy")
		{
			Entity entity = entityManager->GetEntityFromComponent<EntityName>(name);
			m_templates.emplace_back(entity);
		}
	}

	// Spawn initial wave
	SpawnWave();
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
	for (int i = 0; i < 300; i++)
	{
		SpawnEnemy();
	}
}

void EnemySpawnerScript::SpawnEnemy()
{
	int selectedTemplate = 0;
	if (m_templates.size() > 1)
	{
		int maxRandom = m_templates.size() - 1;
		selectedTemplate = rand() % maxRandom;
	}

	// Clone template entity data
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	Entity templateEntity = m_templates[selectedTemplate];
	AnimatedSprite* templateAnimatedSprite = entityManager->GetComponent<AnimatedSprite>(templateEntity);
	Transform* templateTransform = entityManager->GetComponent<Transform>(templateEntity);

	if (m_enemiesAlive < m_maxEnemiesAlive)
	{
		Entity newEntity = entityManager->CreateEntity();

		// Set up animated sprite
		
		std::vector<Texture2D> textures = templateAnimatedSprite->GetFrames();
		float interval = templateAnimatedSprite->GetFrameInterval();
		glm::vec3 colour = templateAnimatedSprite->GetColour();
		Shader* shader = templateAnimatedSprite->GetShader();

		AnimatedSprite* newAnimatedSprite = entityManager->AddComponent<AnimatedSprite>(newEntity, textures, interval, colour, shader);
		
		// Set up name
		entityManager->AddComponent<EntityName>(newEntity, "Enemy");
		
		// Set up transform
		
		Transform* playerTransform = entityManager->GetComponent<Transform>(m_spawnAround);
		Transform* newTransform = entityManager->AddComponent<Transform>(newEntity);
		newTransform->m_position = SetRandomSpawnPos(glm::vec2(300.0f), playerTransform->m_position);
		newTransform->m_rotate = templateTransform->m_rotate;
		newTransform->m_size = templateTransform->m_size;

		// Set up script
		ScriptComponent* scriptC = entityManager->AddComponent<ScriptComponent>(newEntity, entityManager, newEntity);
		if (scriptC)
		{
			scriptC->AttachScript<EnemyMovementScript>(15.0f, m_spawnAround);
		}

		m_enemyEntities.emplace_back(newEntity);
		m_enemiesAlive++;
	}
	else
	{
		Entity replacedEntity = m_enemyEntities[0]; // Replace the oldest entity
		
		entityManager->RemoveComponent<AnimatedSprite>(replacedEntity);

		std::vector<Texture2D> textures = templateAnimatedSprite->GetFrames();
		float interval = templateAnimatedSprite->GetFrameInterval();
		glm::vec3 colour = templateAnimatedSprite->GetColour();
		Shader* shader = templateAnimatedSprite->GetShader();

		AnimatedSprite* newAnimatedSprite = entityManager->AddComponent<AnimatedSprite>(replacedEntity, textures, interval, colour, shader);

		Transform* playerTransform = entityManager->GetComponent<Transform>(m_spawnAround);
		Transform* newTransform = entityManager->GetComponent<Transform>(replacedEntity);
		newTransform->m_position = SetRandomSpawnPos(glm::vec2(300.0f), playerTransform->m_position);
		newTransform->m_rotate = templateTransform->m_rotate;
		newTransform->m_size = templateTransform->m_size;

		m_enemyEntities.erase(m_enemyEntities.begin());
		m_enemyEntities.emplace_back(replacedEntity);
	}
}

glm::vec2 SetRandomSpawnPos(const glm::vec2& viewport, const glm::vec2& centrePos)
{
	int spawnSide = rand() % 4;

	// Offsets from centre to place base spawn location
	float constantOffsetFromEdge = 50.0f;
	float horizontalOffset = viewport.x / 2.0f + constantOffsetFromEdge;
	float verticalOffset = viewport.y / 2.0f + constantOffsetFromEdge;

	// Random offset away from edge
	float awayOffset = rand() % 2;

	// Random offset along edge
	float offsetAlongEdge;

	// Final offsets from centre
	glm::vec2 finalOffset;
	switch (spawnSide)
	{
	case 0: // Left
		offsetAlongEdge = rand() % (int)viewport.y;
		finalOffset.x = -horizontalOffset - awayOffset;
		finalOffset.y = offsetAlongEdge - viewport.y / 2.0f;

		break;
	case 1: // Top
		offsetAlongEdge = rand() % (int)viewport.x;
		finalOffset.x = offsetAlongEdge - viewport.x / 2.0f;
		finalOffset.y = -verticalOffset - awayOffset;

		break;
	case 2: // Right
		offsetAlongEdge = rand() % (int)viewport.y;
		finalOffset.x = horizontalOffset + awayOffset;
		finalOffset.y = offsetAlongEdge - viewport.y / 2.0f;

		break;
	case 3: // Bottom
		offsetAlongEdge = rand() % (int)viewport.x;
		finalOffset.x = offsetAlongEdge - viewport.x / 2.0f;
		finalOffset.y = verticalOffset + awayOffset;

		break;
	}

	return centrePos + finalOffset;
}

Entity EnemySpawnerScript::FindPlayer()
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