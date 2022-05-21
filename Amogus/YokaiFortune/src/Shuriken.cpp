#include "Shuriken.h"
#include "EnemyMovementScript.h"

Shuriken::Shuriken(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level, bool moving, bool autoTarget) : 
	WeaponScript(entityManager, parentEntityID, player, weapon, level, moving, autoTarget)
{
	m_baseProjectileSpeed = 1; //Speed of projectiles
	m_baseProjectileCooldown = 1; //How often weapon attacks
	m_baseProjectileArea = 1; //Size of weapon
	m_baseProjectileDuration = 4; //How long the projectile stays on the screen
	m_baseProjectileCount = 3; //How many projectiles
	m_projectileMax = 15;

	m_baseDamageModifier = 1;

	m_hitDelay = 1;
	m_critMultiplier = 1;

	m_wait = 0.1;

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

		m_manager->AddComponent<Transform>(newProjectile, glm::vec2(1000.0f, 1000.0f), glm::vec2(.25f * m_baseProjectileArea, .25f * m_baseProjectileArea));

		m_manager->AddComponent<Sprite>(newProjectile, m_sprite->GetTexture(), m_sprite->GetColour(), m_sprite->GetShader()); //replace later with animated sprite!
		m_manager->AddComponent<BoxCollider>(newProjectile, transform->m_size, glm::vec2(0.0f)); // Needs a box collider that ignores player?

		glm::vec2 direction(0, 0);

		if (m_isMoving)
		{
			direction = currentPosition - m_playerPreviousPosition;
			glm::normalize(direction);
		}

		if (direction == glm::vec2(0, 0))
			direction = glm::vec2(1, 0);

		Projectiles p;

		p.name = newProjectile;
		p.duration = m_baseProjectileDuration;
		p.direction = direction;
		p.isSpawned = false;

		m_vecProjectiles.push_back(p);
	}


}

Shuriken::~Shuriken()
{
}

void Shuriken::OnAttach()
{
}

void Shuriken::OnRender(float dt)
{
}

void Shuriken::OnUnattach()
{
}

void Shuriken::OnUpdate(float dt)
{
	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		for (int i = 0; i < m_baseProjectileCount; i++)
		{

			SpawnProjectile();

			if (m_currentCooldown <= 0)
			{
				m_isMax = false;
				m_currentCooldown = m_baseProjectileCooldown;
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

			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += ((m_vecProjectiles[i].direction.x * 1000 )* m_baseProjectileSpeed) * dt;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.y += ((m_vecProjectiles[i].direction.y * 1000 )* m_baseProjectileSpeed) * dt;

			auto collisions = g_app->m_collisionManager->potentialCollisions(m_vecProjectiles[i].name);
			for (Entity e : collisions)
			{
				EntityName* name = m_manager->GetComponent<EntityName>(e);
				if (name == NULL)
					continue;

				if (name->m_name == "Enemy")
				{
					if (g_app->m_collisionManager->checkCollision(m_vecProjectiles[i].name, e))
					{
						//TEMPORARY, REPLACE WITH SPAWNING SCRIPT OR SOMETHING IDK
						Entity gem = g_app->m_entityManager->CreateEntity();

						EnemyMovementScript* eScript = (EnemyMovementScript*)g_app->m_entityManager->GetComponent<ScriptComponent>(e)->GetAttachedScript();
						
						g_app->m_entityManager->AddComponent<Transform>(gem, eScript->m_transform->m_position, glm::vec2(1,1));
						g_app->m_entityManager->AddComponent<Sprite>(gem, TextureLoader::CreateTexture2DFromFile("EXP", "Enemies/Exp/16x16/exp1.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));

						m_manager->RemoveComponent<ScriptComponent>(e);
						m_manager->DeleteEntity(e);
					}
				}
			}

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
			direction = glm::vec2(1, 0);
		else
			direction = glm::normalize(direction);
	}

	newProjectile->duration = m_baseProjectileDuration;
	newProjectile->direction = direction;

}