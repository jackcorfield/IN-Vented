#include "WeaponScript.h"
#include <Windows.h>
#include <random>


WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, Entity killCounter,  int level, bool moving, bool autoTarget) :
Script(entityManager, parentEntityID),
m_manager(entityManager),
m_weapon(parentEntityID),
m_player(player),
m_currentLevel(level),
m_isMoving(moving),
m_isAutoTarget(autoTarget),
m_killCounter(killCounter),
m_canLevel(true),
m_icon(nullptr),
m_sprite(nullptr)
{
	m_killCount = 0;
	m_pScript = (PlayerScript*)entityManager->GetComponent<ScriptComponent>(player)->GetAttachedScript();
	m_currentProjectile = 0;
	
	for (Entity e : entityManager->GetAllActiveEntities())
	{
		EntityName* name = entityManager->GetComponent<EntityName>(e);

		if (name)
		{
			if (name->m_name == "XpManager")
			{
				e_xpManager = e;
				m_xpManager = (XpManager*)m_manager->GetComponent<ScriptComponent>(e_xpManager)->GetAttachedScript();
				continue;
			}
			else if (name->m_name == "Enemy Spawner")
			{
				m_eScript = (EnemySpawnerScript*)m_manager->GetComponent<ScriptComponent>(e)->GetAttachedScript();
			}
		}
		
	}
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

bool WeaponScript::OnLevelUp()
{
	if (!m_canLevel)
		return true;

	if (m_currentLevel >= m_maxLevel)
	{
		m_canLevel = false;
		return true;
	}

	switch (m_levelingInfo[m_currentLevel].first)
	{
	case SPEED:
		m_baseProjectileSpeed += (m_baseProjectileSpeed * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case	COOLDOWN:
		m_baseProjectileCooldown += (m_baseProjectileCooldown * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case AREA:
		m_baseProjectileArea += (m_baseProjectileArea * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case	DURATION:
		m_baseProjectileDuration += (m_baseProjectileDuration * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case	COUNT:
		m_baseProjectileCount += m_levelingInfo[m_currentLevel].second;
		break;
	case DAMAGE:
		m_baseDamageModifier += m_levelingInfo[m_currentLevel].second;
		break;
	}

	m_currentLevel++;
	m_pScript->UpdateLevel(m_elementNum -1, m_currentLevel+1);
	return false;
}

bool WeaponScript::CheckWeaponCollision(Entity weaponID, bool areaOfEffect)
{
	bool collisionDetected = false;
	auto collisions = g_app->m_collisionManager->potentialCollisions(weaponID);
	for (Entity e : collisions)
	{
		EntityName* name = m_manager->GetComponent<EntityName>(e);
		if (name == NULL)
			continue;

		if (name->m_name == "Enemy")
		{
			if (g_app->m_collisionManager->checkCollision(weaponID, e))
			{
				m_xpManager->SpawnOrb(m_manager->GetComponent<Transform>(e)->m_position, 100);

				//Handle Enemy Killing
			//	m_manager->RemoveComponent<ScriptComponent>(e);
			//	m_manager->DeleteEntity(e);
				//m_manager->GetComponent<Transform>(e)->m_position = glm::vec2(-9999999, -9999999);
				//m_eScript = (EnemySpawnerScript*)m_manager->GetComponent<ScriptComponent>(e)->GetAttachedScript();
				m_eScript->KillEnemy(e);
				m_killCount++;
				m_manager->GetComponent<UI_WidgetComponent>(m_killCounter)->GetText("killText")->m_text = std::to_string(m_killCount);

				if (!areaOfEffect)
					return true;

				collisionDetected = true;
			}
		}
	}

	return collisionDetected;
}

void WeaponScript::OnAttach()
{
}

void WeaponScript::OnRender(float dt)
{
	
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