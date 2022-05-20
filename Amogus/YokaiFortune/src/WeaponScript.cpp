#include "WeaponScript.h"
#include <Windows.h>
#include <random>


WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon,  int level, bool moving, bool autoTarget) :
Script(entityManager, parentEntityID),
m_manager(entityManager),
m_weapon(parentEntityID),
m_player(player),
m_currentLevel(level),
m_isMoving(moving),
m_isAutoTarget(autoTarget),
m_canLevel(true),
m_icon(nullptr),
m_sprite(nullptr)
{
	m_currentProjectile = 0;
}

WeaponScript::~WeaponScript()
{
}

void WeaponScript::SetSprites(Sprite* icon, Sprite* sprite)
{
	m_icon = icon;
	m_sprite = sprite;
	m_hitboxSize = glm::vec2(m_sprite->GetTexture().m_width, m_sprite->GetTexture().m_height);
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

void WeaponScript::OnRender(float dt)
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

void WeaponScript::OnUnattach()
{
}

void WeaponScript::OnUpdate(float dt)
{
	
}

void WeaponScript::SpawnProjectile()
{
	
}