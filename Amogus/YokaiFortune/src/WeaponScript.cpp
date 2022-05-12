#include "WeaponScript.h"
#include <Windows.h>

WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, Sprite icon, Sprite sprite, glm::vec2 hitboxSize, int level, bool moving) :
Script(entityManager, parentEntityID),
m_manager(entityManager),
m_player(parentEntityID),
m_icon(icon),
m_sprite(sprite),
m_currentLevel(level),
m_IsMoving(moving),
m_canLevel(true)
{
	 m_baseProjectileSpeed = 10; //Speed of projectiles
	 m_baseProjectileCooldown = 1; //How often weapon attacks
	 m_baseProjectileArea = 1; //Size of weapon
	 m_baseProjectileDuration = 0.4; //How long the projectile stays on the screen
	 m_baseProjectileCount = 2; //How many projectiles
	 m_projectileMax = 2;

	 m_baseDamageModifier = 1;

	 m_hitDelay = 1;
	 m_critMultiplier = 1;

	 m_wait = 0.1;
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
	m_currentCooldown -= dt;
	m_wait -= dt;
	if (!m_isMax && m_wait <= 0)
	{
		SpawnProjectile();
		m_wait = 0.1f;
	}

	if (m_vecProjectiles.size() == m_baseProjectileCount)
		m_isMax = true;

	if (m_currentCooldown <= 0)
	{
		m_isMax = false;
		m_currentCooldown = m_baseProjectileCooldown;
	}
	
	if (m_vecProjectiles.size() <= 0)
		return;

	for (int i = 0; i < m_vecProjectiles.size(); i++)
	{
		if (m_vecProjectiles[i].second > 0)
		{
			if (!m_IsMoving)
			{
				continue;
			}
			
			//TEMPORARY
			//will need to be replaced with a more dynamic system, such as moving the same direction as the player is facing, diagonal shooting etc
		
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].first)->m_position.x += 100 * dt;

			//TEMPORARY	

			m_vecProjectiles[i].second -= dt;
		}
		else
		{
			Entity entity = m_vecProjectiles[i].first;
			//m_manager->DeleteEntity(projectile->first);
			if (m_vecProjectiles[i] != m_vecProjectiles[0])
				m_vecProjectiles.erase(m_vecProjectiles.begin() + i);
			else
				m_vecProjectiles.erase(m_vecProjectiles.begin()+i);

			m_manager->DeleteEntity(entity);
			i--;
		}
	}

	//COLLISION SHOULD BE TAKING PLACE IN CHECK COLLISION FUNCTION : )
}

void WeaponScript::OnRender(float dt)
{
}

void WeaponScript::OnUnattach()
{
}

void WeaponScript::SpawnProjectile()
{

	Entity newProjectile = m_manager->CreateEntity();

	Transform* transform = GetComponent<Transform>();

	//m_manager->AddComponent<Transform>(newProjectile, startLocation->m_position, m_hitboxSize);
	m_manager->AddComponent<Transform>(newProjectile, glm::vec2(0,0), glm::vec2(.1f * m_baseProjectileArea,.1f * m_baseProjectileArea));
	// m_manager->AddComponent<Texture2D>(newProjectile);
	m_manager->AddComponent<Sprite>(newProjectile, m_sprite.GetTexture(), m_sprite.GetColour(), m_sprite.GetShader()); //replace later with animated sprite!
	m_manager->AddComponent<BoxCollider>(newProjectile, transform->m_size); // Needs a box collider that ignores player?
	
	if (m_IsMoving)
	{
		//set move direction
	}

	m_vecProjectiles.push_back(std::make_pair(newProjectile, m_baseProjectileDuration));

	//play noise
}