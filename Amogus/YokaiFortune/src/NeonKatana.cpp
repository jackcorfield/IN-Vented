#include "NeonKatana.h"

NeonKatana::NeonKatana(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, Entity m_killCounter, int level, bool moving, bool autoTarget) :
	WeaponScript(entityManager, parentEntityID, player, weapon, m_killCounter, level, moving, autoTarget)
{
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 5));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 5));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));

	m_maxLevel = m_levelingInfo.size();

	m_baseProjectileSpeed = 1; //Speed of projectiles
	m_baseProjectileCooldown = 1.5; //How often weapon attacks
	m_baseProjectileArea = .7; //Size of weapon
	m_baseProjectileDuration = .1f; //How long the projectile stays on the screen
	m_baseProjectileCount = 2; //How many projectiles
	m_projectileMax = 20;

	m_baseDamageModifier = 1;

	m_hitDelay = 1;
	m_critMultiplier = 1;

	m_wait = 0.1;

	m_playerPreviousPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	std::srand(time(NULL));

	Sprite* icon = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("NeonKatanaIcon", "Weapons/NeonKatana/neonkatana.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	Sprite* sprite = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("NeonKatanaAnimation", "Weapons/NeonKatana/Animation/frame5.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));

	SetSprites(icon, sprite);

	m_audio = entityManager->AddComponent<Audio>(weapon, "sfx/Weapons/neonkatana.wav", g_app->m_audioManager->m_system, g_app->m_audioManager->m_sfx);

	for (int i = 0; i < m_projectileMax; i++)
	{
		Entity newProjectile = m_manager->CreateEntity();

		Transform* transform = GetComponent<Transform>();

		glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

		float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;

		m_originalTransformSize = glm::vec2(.25f * (m_baseProjectileArea + PercentageIncrease));
		m_manager->AddComponent<Transform>(newProjectile, glm::vec2(1000.0f, 1000.0f), m_originalTransformSize);

		m_manager->AddComponent<Sprite>(newProjectile, m_sprite->GetTexture(), m_sprite->GetColour(), m_sprite->GetShader()); //replace later with animated sprite!

		m_originalBoxSize = glm::vec2(60, 70);
		m_originalBoxOffset = glm::vec2(0.0f, 25.0f);
		m_manager->AddComponent<BoxCollider>(newProjectile, m_originalBoxSize, m_originalBoxOffset);

		glm::vec2 direction(0, 0);

		Projectiles p;

		PercentageIncrease = (m_baseProjectileDuration * m_pScript->m_projectileDuration) / 100;

		p.name = newProjectile;
		p.duration = m_baseProjectileDuration + PercentageIncrease;
		p.direction = direction;
		p.isSpawned = false;

		m_vecProjectiles.push_back(p);
	}

	m_elementNum = m_pScript->AddWeapon(icon, level);
}

NeonKatana::~NeonKatana()
{
}

void NeonKatana::OnAttach()
{
}

void NeonKatana::OnUpdate(float dt)
{
	if (m_pScript->m_isDead)
	{
		m_manager->DeleteEntity(m_weapon);
	}


	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		for (int i = 0; i < m_baseProjectileCount +m_pScript->m_projectileCount; i++)
		{

			SpawnProjectile(i);

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

			float PercentageSpeedIncrease = (m_baseProjectileSpeed * m_pScript->m_projectileSpeed) / 100;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += (m_vecProjectiles[i].direction.x * 1000) * (m_baseProjectileSpeed + PercentageSpeedIncrease) * dt;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.y += (m_vecProjectiles[i].direction.y * 1000) * (m_baseProjectileSpeed + PercentageSpeedIncrease) * dt;

			float PercentageAreaIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;

			Transform* transform = m_manager->GetComponent<Transform>(m_vecProjectiles[i].name);
			transform->m_size = m_originalTransformSize * (m_baseProjectileArea + PercentageAreaIncrease);

			BoxCollider* boxCollider = m_manager->GetComponent<BoxCollider>(m_vecProjectiles[i].name);
			boxCollider->m_size = m_originalBoxSize * (m_baseProjectileArea + PercentageAreaIncrease);
			boxCollider->m_offset = m_originalBoxOffset * ((m_baseProjectileArea + PercentageAreaIncrease) / 2.0f);

			CheckWeaponCollision(m_vecProjectiles[i].name, true);

			m_vecProjectiles[i].duration -= dt;
		}
		else
		{
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position = glm::vec2(1000.0f, 1000.0f);
			m_vecProjectiles[i].isSpawned = false;
			i--;
		}
	}

	m_playerPreviousPosition = currentPosition;
}

void NeonKatana::OnRender(float dt)
{
}

void NeonKatana::OnUnattach()
{
}

void NeonKatana::SpawnProjectile(int currentPos)
{

	Projectiles* newProjectile = &m_vecProjectiles[m_currentProjectile];
	newProjectile->isSpawned = true;

	m_currentProjectile++;
	if (m_currentProjectile >= m_vecProjectiles.size() - 1)
		m_currentProjectile = 0;

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	//set sprite to one direction

	if (currentPos % 2 == 0)
	{
		//even -> right
		m_manager->GetComponent<Transform>(newProjectile->name)->m_position = currentPosition + glm::vec2(-22, -22* currentPos);
		
	}
	else
	{
		//odd -> left
		m_manager->GetComponent<Transform>(newProjectile->name)->m_position = currentPosition + glm::vec2(22, 22*-currentPos);
		m_manager->GetComponent<Transform>(newProjectile->name)->m_size.x = m_manager->GetComponent<Transform>(newProjectile->name)->m_size.x * -1;
	}

	float PercentageIncrease = (m_baseProjectileDuration * m_pScript->m_projectileDuration) / 100;
	newProjectile->duration = m_baseProjectileDuration + PercentageIncrease;

}
