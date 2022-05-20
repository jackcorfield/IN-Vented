#include "WeaponScript.h"
#include <Windows.h>
#include <random>


WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Sprite icon, Sprite sprite, glm::vec2 hitboxSize, int level, bool moving, bool autoTarget) :
Script(entityManager, parentEntityID),
m_manager(entityManager),
m_weapon(parentEntityID),
m_player(player),
m_icon(icon),
m_sprite(sprite),
m_currentLevel(level),
m_isMoving(moving),
m_isAutoTarget(autoTarget),
m_canLevel(true)
{
	 m_baseProjectileSpeed = 10; //Speed of projectiles
	 m_baseProjectileCooldown = 1; //How often weapon attacks
	 m_baseProjectileArea = 1; //Size of weapon
	 m_baseProjectileDuration = 4; //How long the projectile stays on the screen
	 m_baseProjectileCount = 3; //How many projectiles
	 m_projectileMax = 6;

	 m_baseDamageModifier = 1;

	 m_hitDelay = 1;
	 m_critMultiplier = 1;

	 m_wait = 0.1;

	 m_playerPreviousPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	 std::srand(time(NULL));
}

WeaponScript::~WeaponScript()
{
}

void WeaponScript::OnLevelUp()
{
	if (!m_canLevel)
		return;

	if (m_currentLevel >= m_maxLevel)
	{
		m_canLevel = false;
		return;
	}

	m_currentLevel++;

	switch (m_levelingInfo[m_currentLevel].first)
	{
	case SPEED:
		m_baseProjectileSpeed += m_levelingInfo[m_currentLevel].second;
		break;
	case	COOLDOWN:
		m_baseProjectileCooldown += m_levelingInfo[m_currentLevel].second;
		break;
	case AREA:
		m_baseProjectileArea += m_levelingInfo[m_currentLevel].second;
		break;
	case	DURATION:
		m_baseProjectileDuration += m_levelingInfo[m_currentLevel].second;
		break;
	case	COUNT:
		m_baseProjectileCount += m_levelingInfo[m_currentLevel].second;
		break;
	case DAMAGE:
		m_baseDamageModifier += m_levelingInfo[m_currentLevel].second;
		break;
	}
}

void WeaponScript::OnAttach()
{
}

void WeaponScript::OnUpdate(float dt)
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
	}
	
	if (m_vecProjectiles.size() <= 0)
		return;

	for (int i = 0; i < m_vecProjectiles.size(); i++)
	{
		if (m_vecProjectiles[i].duration > 0)
		{
			if (!m_isMoving)
			{
				continue;
			}
			
			//TEMPORARY
			//will need to be replaced with a more dynamic system, such as moving the same direction as the player is facing, diagonal shooting etc
		

			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += (m_vecProjectiles[i].direction.x* 1000) * dt;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.y += (m_vecProjectiles[i].direction.y * 1000) * dt;
			//m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += 100 * dt;

			//TEMPORARY	

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
						m_manager->RemoveComponent<ScriptComponent>(e);
						m_manager->DeleteEntity(e);
					}
				}
			}
			
			m_vecProjectiles[i].duration -= dt;
		}
		else
		{
			Entity entity = m_vecProjectiles[i].name;
			m_vecProjectiles.erase(m_vecProjectiles.begin() + i);
			m_manager->DeleteEntity(entity);
			i--;
		}
	}

	m_playerPreviousPosition = currentPosition;
}

void WeaponScript::OnRender(float dt)
{
}

void WeaponScript::OnUnattach()
{
}

void WeaponScript::SpawnProjectile()
{

	glm::vec2 offset;

	offset.x = (rand() % 200 - 100)/2;
	offset.y = (rand() % 200 - 100)/2;

	//MAKE BASE SET
	//IF MORE ARE NEEDED (EG THE CURRENT ARE STILL IN USE)
	//ADD MORE ENTITIES TO THE LIST
	//IF NOT, REUSE OLD ENTITIES

	Entity newProjectile = m_manager->CreateEntity();

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_manager->AddComponent<Transform>(newProjectile, glm::vec2(0,0), glm::vec2(.25f * m_baseProjectileArea,.25f * m_baseProjectileArea));

	m_manager->GetComponent<Transform>(newProjectile)->m_position =  currentPosition + offset;

	m_manager->AddComponent<Sprite>(newProjectile, m_sprite.GetTexture(), m_sprite.GetColour(), m_sprite.GetShader()); //replace later with animated sprite!
	m_manager->AddComponent<BoxCollider>(newProjectile, transform->m_size); // Needs a box collider that ignores player?
	
	glm::vec2 direction(0, 0);
	
	if (m_isMoving)
	{
		direction =  currentPosition - m_playerPreviousPosition;
		glm::normalize(direction);
	}

	if (direction == glm::vec2(0, 0))
		direction = glm::vec2(1, 0);

	Projectiles p;

	p.name = newProjectile;
	p.duration = m_baseProjectileDuration;
	p.direction = direction;

	m_vecProjectiles.push_back(p);

	//play noise
}