#include "WeaponScript.h"


WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, int level, bool moving) :Script(entityManager, parentEntityID),
m_manager(entityManager),
m_player(parentEntityID),
m_canLevel(true),
m_IsMoving(moving),
m_currentLevel(level)
{
	//AddComponent<Sprite>(m_player, newTexture, newColour, newShader);
	//AddComponent<BoxCollider>(m_player, (GetComponent<Sprite>()->GetTexture().m_width, GetComponent<Sprite>()->GetTexture().m_height));
}

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
	
	if (m_IsMoving)
	{
		//move projectile according to speed
		///transform += m_baseProjectileSpeed*dt;
	}

	//for all projectiles in list, check duration, if >= 0, remove

	//check for collision
}

void WeaponScript::SpawnProjectile()
{
	//get sprite
	// 
	//set box collider
	// 
	//set move direction ( if projectile moves)
	// 
	//add to list of projectiles + duration (Pair?)
}