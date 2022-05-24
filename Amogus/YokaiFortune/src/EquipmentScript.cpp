#include "EquipmentScript.h"
#include <Windows.h>
#include <random>

EquipmentScript::EquipmentScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level) :
	Script(entityManager, parentEntityID),
	m_manager(entityManager),
	m_equipment(parentEntityID),
	m_player(player),
	m_icon(nullptr)
{
	m_pScript = (PlayerScript*)entityManager->GetComponent<ScriptComponent>(player)->GetAttachedScript();
}

EquipmentScript::~EquipmentScript()
{
}

void EquipmentScript::SetSprites(Sprite* icon)
{
	m_icon = icon;
}

void EquipmentScript::OnAttach()
{
}

void EquipmentScript::OnUpdate(float dt)
{
}

void EquipmentScript::OnRender(float dt)
{
}

void EquipmentScript::OnUnattach()
{
}

void EquipmentScript::OnLevelUp()
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
		m_pScript->m_projectileSpeed += (m_pScript->m_projectileSpeed * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case	COOLDOWN:
		m_pScript->m_projectileCooldown += (m_pScript->m_projectileCooldown * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case AREA:
		m_pScript->m_projectileArea += (m_pScript->m_projectileCooldown * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case	DURATION:
		m_pScript->m_projectileDuration += (m_pScript->m_projectileCooldown * m_levelingInfo[m_currentLevel].second) / 100;
		break;
	case	COUNT:
		m_pScript->m_projectileCount += m_levelingInfo[m_currentLevel].second;
		break;
	}

	m_pScript->UpdateLevel(m_elementNum, m_currentLevel);
}
