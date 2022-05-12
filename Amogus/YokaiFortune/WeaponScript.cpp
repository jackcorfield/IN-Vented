#include "WeaponScript.h"


WeaponScript::WeaponScript(EntityManager* entityManager, Entity parentEntityID, int level) :Script(entityManager, parentEntityID),
m_manager(entityManager),
m_player(parentEntityID),
m_canLevel(true),
m_currentLevel(level)
{
	//AddComponent<Sprite>(m_player, newTexture, newColour, newShader);
	//AddComponent<BoxCollider>(m_player, (GetComponent<Sprite>()->GetTexture().m_width, GetComponent<Sprite>()->GetTexture().m_height));
}

void WeaponScript::OnLevelUp()
{
	if (!m_currentLevel < m_maxLevel)
		return;

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
