#include "WeaponScript.h"


WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, Sprite icon, Sprite sprite, glm::vec2 hitboxSize, int level, bool moving) :Script(entityManager, parentEntityID),
m_manager(entityManager),
m_player(parentEntityID),
m_icon(icon),
m_sprite(sprite),
m_hitboxSize(m_hitboxSize),
m_currentLevel(level),
m_IsMoving(moving),
m_canLevel(true)
{}

void WeaponScript::OnLevelUp()
{
	if (!m_canLevel)
		return;

	if (!m_currentLevel < m_maxLevel)
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

void WeaponScript::OnUpdate(float dt)
{
	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		for (int i = 0; i < m_baseProjectileCount; i++)
		{
			SpawnProjectile();
			//add a delay?
		}

		m_currentCooldown = m_baseProjectileCooldown;
	}
	
	for (auto projectile = m_vecProjectiles.begin(); projectile != m_vecProjectiles.end(); projectile++)
	{
		
		if (projectile->second >= 0)
		{
			if (!m_IsMoving)
			{
				continue;
			}
			
			//TEMPORARY
			//will need to be replaced with a more dynamic system, such as moving the same direction as the player is facing, diagonal shooting etc
		
			m_manager->GetComponent<Transform>(projectile->first)->m_position+= m_baseProjectileSpeed * dt;

			//TEMPORARY	

			projectile->second -= dt;
		}
		else
		{
			m_manager->DeleteEntity(projectile->first);
			m_vecProjectiles.erase(projectile--);
		}
	}

	//COLLISION SHOULD BE TAKING PLACE IN CHECK COLLISION FUNCTION : )
}

void WeaponScript::SpawnProjectile()
{

	Entity newProjectile = m_manager->CreateEntity();

	Transform* startLocation = GetComponent<Transform>();
	
	m_manager->AddComponent<Transform>(newProjectile, startLocation->m_position, startLocation->m_size);
	m_manager->AddComponent<Sprite>(newProjectile, m_sprite.GetTexture(), m_sprite.GetColour(), m_sprite.GetShader()); //replace later with animated sprite!
	m_manager->AddComponent<BoxCollider>(newProjectile, startLocation->m_size); // Needs a box collider that ignores player?
	
	if (m_IsMoving)
	{
		//set move direction
	}

	m_vecProjectiles.push_back(std::make_pair(newProjectile, m_baseProjectileDuration));

	//play noise
}