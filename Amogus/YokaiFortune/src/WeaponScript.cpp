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