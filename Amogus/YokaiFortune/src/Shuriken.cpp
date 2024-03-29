#include "Shuriken.h"

Shuriken::Shuriken(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, Entity m_killCounter, int level, bool moving, bool autoTarget) :
	WeaponScript(entityManager, parentEntityID, player, weapon, m_killCounter, level, moving, autoTarget)
{
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));

	m_maxLevel = m_levelingInfo.size();


	m_baseProjectileSpeed = 0.5; //Speed of projectiles
	m_baseProjectileCooldown = 1; //How often weapon attacks
	m_baseProjectileArea = 0.7f; //Size of weapon
	m_baseProjectileDuration = 4; //How long the projectile stays on the screen
	m_baseProjectileCount = 3; //How many projectiles
	m_projectileMax = 15;

	m_baseDamageModifier = 1;

	m_hitDelay = 1;
	m_critMultiplier = 1;

	m_wait = 0.1;

	m_previousDirection = glm::vec2(1, 0);

	m_playerPreviousPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	std::srand(time(NULL));

	Sprite* sprite = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("ShurikenIconSprite", "Weapons/Shuriken/Shuriken.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(sprite, sprite);

	m_audio = entityManager->AddComponent<Audio>(weapon, "sfx/Weapons/shuriken.mp3", g_app->m_audioManager->m_system, g_app->m_audioManager->m_sfx);

	for (int i = 0; i < m_projectileMax; i++)
	{
		Entity newProjectile = m_manager->CreateEntity();

		Transform* transform = GetComponent<Transform>();

		glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

		float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;

		m_originalTransformSize = glm::vec2(.25f * (m_baseProjectileArea + PercentageIncrease));
		m_manager->AddComponent<Transform>(newProjectile, glm::vec2(1000.0f, 1000.0f), m_originalTransformSize);

		m_manager->AddComponent<Sprite>(newProjectile, m_sprite->GetTexture(), m_sprite->GetColour(), m_sprite->GetShader()); //replace later with animated sprite!

		m_originalBoxSize = transform->m_size;
		m_originalBoxOffset = glm::vec2(0.0f);
		m_manager->AddComponent<BoxCollider>(newProjectile, m_originalBoxSize, m_originalBoxOffset);

		glm::vec2 direction(0, 0);

		if (m_isMoving)
		{
			direction = currentPosition - m_playerPreviousPosition;
			glm::normalize(direction);
		}

		if (direction == glm::vec2(0, 0))
			direction = glm::vec2(1, 0);

		Projectiles p;

		PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileDuration) / 100;

		p.name = newProjectile;
		p.duration = m_baseProjectileDuration + PercentageIncrease;
		p.direction = direction;
		p.isSpawned = false;

		m_vecProjectiles.push_back(p);
	}

	m_elementNum = m_pScript->AddWeapon(sprite, level);
}

Shuriken::~Shuriken()
{
}

void Shuriken::OnAttach()
{
}

void Shuriken::OnRender(float dt)
{
	Transform* transform;
	for (Projectiles proj : m_vecProjectiles)
	{
		if (proj.isSpawned)
		{
			transform = m_manager->GetComponent<Transform>(proj.name);
			transform->m_rotate += dt * 360;
		}
	}

}

void Shuriken::OnUnattach()
{
}

void Shuriken::OnUpdate(float dt)
{
	if (m_pScript->m_isDead)
	{
		m_manager->DeleteEntity(m_weapon);
	}


	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		for (int i = 0; i < m_baseProjectileCount + m_pScript->m_projectileCount; i++)
		{

			SpawnProjectile();

			if (m_currentCooldown <= 0)
			{
				m_isMax = false;
				float percentageReduction = (m_baseProjectileCooldown * m_pScript->m_projectileCooldown) / 100;
				m_currentCooldown = m_baseProjectileCooldown - percentageReduction;
			}
		}

		g_app->m_audioManager->m_system->playSound(m_audio->m_sound, m_audio->m_group, false, &m_audio->m_channel);
	}

	if (m_vecProjectiles.size() <= 0)
		return;

	for (int i = 0; i < m_vecProjectiles.size(); i++)
	{
		if (!m_vecProjectiles[i].isSpawned)
			continue;

		if (m_vecProjectiles[i].duration > 0)
		{
			if (!m_isMoving)
				continue;

			float PercentageIncrease = (m_baseProjectileSpeed * m_pScript->m_projectileSpeed) / 100;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += (m_vecProjectiles[i].direction.x * 1000) * (m_baseProjectileSpeed + PercentageIncrease) * dt;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.y += (m_vecProjectiles[i].direction.y * 1000) * (m_baseProjectileSpeed + PercentageIncrease) * dt;

			float PercentageAreaIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;

			Transform* transform = m_manager->GetComponent<Transform>(m_vecProjectiles[i].name);
			transform->m_size = m_originalTransformSize * (m_baseProjectileArea + PercentageAreaIncrease);

			BoxCollider* boxCollider = m_manager->GetComponent<BoxCollider>(m_vecProjectiles[i].name);
			boxCollider->m_size = m_originalBoxSize * (m_baseProjectileArea + PercentageAreaIncrease);
			boxCollider->m_offset = m_originalBoxOffset * ((m_baseProjectileArea + PercentageAreaIncrease) / 2.0f);

			//Check collions
			if(CheckWeaponCollision(m_vecProjectiles[i].name))
				m_vecProjectiles[i].duration = 0;

			m_vecProjectiles[i].duration -= dt;
		}
		else
		{
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position = glm::vec2(1000.0f, 1000.0f);
			m_vecProjectiles[i].isSpawned = false;
			i--;
		}
	}

	//COLLISION SHOULD BE TAKING PLACE IN CHECK COLLISION FUNCTION : )

	m_playerPreviousPosition = currentPosition;
}

void Shuriken::SpawnProjectile()
{
	glm::vec2 offset;

	offset.x = (rand() % 200 - 100) / 2;
	offset.y = (rand() % 200 - 100) / 2;

	Projectiles* newProjectile = &m_vecProjectiles[m_currentProjectile];
	newProjectile->isSpawned = true;
	

	m_currentProjectile++;
	if (m_currentProjectile >= m_vecProjectiles.size() - 1)
		m_currentProjectile = 0;

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_manager->GetComponent<Transform>(newProjectile->name)->m_position = currentPosition + offset;

	glm::vec2 direction(0, 0);

	if (m_isMoving)
	{
		direction = currentPosition - m_playerPreviousPosition;
		
		if (direction == glm::vec2(0, 0))
			direction = m_previousDirection;
		else
		{
			direction = glm::normalize(direction);
			m_previousDirection = direction;
		}
			
	}

	float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileDuration) / 100;

	newProjectile->duration = m_baseProjectileDuration + PercentageIncrease;
	newProjectile->direction = direction;

}