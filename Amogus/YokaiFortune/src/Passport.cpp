#include "Passport.h"

Passport::Passport(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level) :
	EquipmentScript(entityManager, parentEntityID, player, equipment, level)
{
	Sprite* icon = entityManager->AddComponent<Sprite>(parentEntityID, TextureLoader::CreateTexture2DFromFile("PassportIcon", "Equipment/Passport.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(icon);

	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));
	m_levelingInfo.push_back(std::make_pair(AREA, 10));

	m_maxLevel = m_levelingInfo.size();

	m_canLevel = true;
	m_currentLevel = level;

	OnLevelUp();
	m_pScript->AddEquip(icon);
}