#include "PowerGem.h"

PowerGem::PowerGem(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level) :
	EquipmentScript(entityManager, parentEntityID, player, equipment, level)
{
	Sprite* icon = entityManager->AddComponent<Sprite>(parentEntityID, TextureLoader::CreateTexture2DFromFile("PowerGemIcon", "Equipment/PowerGem.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(icon);

	m_levelingInfo.push_back(std::make_pair(DURATION, 10));
	m_levelingInfo.push_back(std::make_pair(DURATION, 10));
	m_levelingInfo.push_back(std::make_pair(DURATION, 10));
	m_levelingInfo.push_back(std::make_pair(DURATION, 10));
	m_levelingInfo.push_back(std::make_pair(DURATION, 10));

	m_currentLevel = level;

	OnLevelUp();

	m_maxLevel = m_levelingInfo.size();
}