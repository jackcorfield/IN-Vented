#include "Ragnite.h"

Ragnite::Ragnite(EntityManager* entityManager, Entity parentEntityID, Entity player, int level) :
	EquipmentScript(entityManager, parentEntityID, player, level)
{
	Sprite* icon = entityManager->AddComponent<Sprite>(parentEntityID, TextureLoader::CreateTexture2DFromFile("RagniteIcon", "Equipment/Ragnite.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(icon);

	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 8));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 8));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 8));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 8));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 8));

	m_currentLevel = level;

	OnLevelUp();

	m_maxLevel = m_levelingInfo.size();
}
