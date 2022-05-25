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
	m_timeBetweenWaveSpawns(60.0f),
	m_timeBetweenSeeks(0.5f),
	m_timeElapsedSinceLastSeek(m_timeBetweenSeeks)
{}

void EnemySpawnerScript::OnAttach()
{
	// If no player ID was provided in constructor, try to find player to spawn enemies around
	if (m_spawnAround == 0) { m_spawnAround = FindPlayer(); }

	// Fill template vector
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	auto allNames = entityManager->GetAllComponentsOfType<EntityName>();
	for (EntityName* name : allNames)
	{
		if (name->m_name == "EnemyTemplate")
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
	m_timeElapsedSinceLastSeek += dt;

	if (m_timeElapsedSinceLastWave >= m_timeBetweenWaveSpawns)
	{
		SpawnWave();
		m_timeElapsedSinceLastWave = 0.0f;
	}

	if (m_timeElapsedSinceLastSeek >= m_timeBetweenSeeks)
	{
		CalculateAllDirections();
		m_timeElapsedSinceLastSeek = 0.0f;
	}

	// If there is no target entity, try to find one. If there is still no target, return
	if (m_spawnAround == 0) { m_spawnAround = FindPlayer(); }
	if (m_spawnAround == 0) { return; }
}

void EnemySpawnerScript::OnRender(float dt)
{}

void EnemySpawnerScript::OnUnattach()
{}

void EnemySpawnerScript::CalculateAllDirections()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	glm::vec2 playerPos = entityManager->GetComponent<Transform>(m_spawnAround)->m_position;

	for (Entity enemy : m_enemyEntities)
	{
		CalculateDir(enemy, entityManager, playerPos);
	}
}

void EnemySpawnerScript::CalculateDir(Entity entity, EntityManager* entityManager, const glm::vec2& playerPos)
{
	ScriptComponent* scriptC = entityManager->GetComponent<ScriptComponent>(entity);
	if (!scriptC) { return; }
	EnemyMovementScript* script = reinterpret_cast<EnemyMovementScript*>(scriptC->GetAttachedScript());

	Transform* enemyTransform = entityManager->GetComponent<Transform>(entity);

	glm::vec2 newDir = Seek(enemyTransform->m_position, playerPos);
	script->SetDirection(newDir);
}

glm::vec2 EnemySpawnerScript::Seek(glm::vec2 pos, glm::vec2 seekTo)
{
	return glm::normalize(seekTo - pos);
}

void EnemySpawnerScript::SpawnWave()
{
	for (int i = 0; i < 100; i++)
	{
		SpawnEnemy();
	}

	CalculateAllDirections();
}

void EnemySpawnerScript::SpawnEnemy()
{
	int selectedTemplate = 0;
	if (m_templates.size() > 1)
	{
		int maxRandom = m_templates.size();
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

		CloneEnemy(templateEntity, newEntity);

		// New entities need a script set up, so do that
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

		// Remove animated sprite so it can be replaced
		entityManager->RemoveComponent<AnimatedSprite>(replacedEntity);

		CloneEnemy(templateEntity, replacedEntity);

		m_enemyEntities.erase(m_enemyEntities.begin());
		m_enemyEntities.emplace_back(replacedEntity);
	}
}

void EnemySpawnerScript::CloneEnemy(Entity templateEntity, Entity targetEntity)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	AnimatedSprite* templateAnimatedSprite = entityManager->GetComponent<AnimatedSprite>(templateEntity);
	Transform* templateTransform = entityManager->GetComponent<Transform>(templateEntity);
	BoxCollider* templateCollider = entityManager->GetComponent<BoxCollider>(templateEntity);

	// Set up animated sprite
	Texture2D texture = templateAnimatedSprite->GetTexture();
	glm::vec3 colour = templateAnimatedSprite->GetColour();
	Shader* shader = templateAnimatedSprite->GetShader();
	glm::vec2 frameSize = templateAnimatedSprite->getFrameSize();

	AnimatedSprite* newAnimatedSprite = entityManager->AddComponent<AnimatedSprite>(targetEntity, texture, frameSize, colour, shader);

	// Set up animations
	std::map<std::string, Animation> animations = templateAnimatedSprite->getAnimations();
	for (auto animationItr = animations.begin(); animationItr != animations.end(); animationItr++)
	{
		std::string name = animationItr->first;
		float frameTime = animationItr->second.frameTime;
		std::vector<unsigned int> frames = animationItr->second.frames;
		newAnimatedSprite->createAnimation(name, frames, frameTime);
	}
	newAnimatedSprite->setAnimation(animations.begin()->first);

	// Set up name
	entityManager->AddComponent<EntityName>(targetEntity, "Enemy");

	// Get camera internal viewport so we can spawn enemies offscreen
	Camera* camera = g_app->m_sceneManager->GetActiveScene()->m_entityManager->GetAllComponentsOfType<Camera>()[0];
	glm::vec2 viewportSize(300.0f);
	if (camera)
	{
		viewportSize = glm::vec2(camera->m_internalWidth, camera->m_internalHeight);
	}

	// Set up transform
	Entity cameraEntity = entityManager->GetEntityFromComponent<Camera>(camera);
	Transform* cameraTransform = entityManager->GetComponent<Transform>(cameraEntity);
	Transform* playerTransform = entityManager->GetComponent<Transform>(m_spawnAround);
	Transform* newTransform = entityManager->AddComponent<Transform>(targetEntity);
	newTransform->m_position = SetRandomSpawnPos(viewportSize, cameraTransform->m_position);
	newTransform->m_rotate = templateTransform->m_rotate;
	newTransform->m_size = templateTransform->m_size;
	newTransform->m_depth = 0.0f;

	// Set up box collider
	BoxCollider* newCollider = entityManager->AddComponent<BoxCollider>(targetEntity, templateCollider->m_size, templateCollider->m_offset);
}

glm::vec2 SetRandomSpawnPos(const glm::vec2& viewport, const glm::vec2& centrePos)
{
	int spawnSide = rand() % 4;

	// Offsets from centre to place base spawn location
	float constantOffsetFromEdge = 50.0f;
	float horizontalOffset = viewport.x / 2.0f + constantOffsetFromEdge;
	float verticalOffset = viewport.y / 2.0f + constantOffsetFromEdge;

	// Random offset away from edge
	float awayOffset = rand() % 30;

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