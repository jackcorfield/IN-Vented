#include "OpticalImplant.h"

OpticalImplant::OpticalImplant(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level) :
	EquipmentScript(entityManager, parentEntityID, player, equipment, level)
{
	Sprite* icon = entityManager->AddComponent<Sprite>(parentEntityID, TextureLoader::CreateTexture2DFromFile("OpticalIcon", "Equipment/OpticalImplant.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(icon);

	m_levelingInfo.push_back(std::make_pair(COUNT, 1));
	m_levelingInfo.push_back(std::make_pair(COUNT, 1));

	m_maxLevel = m_levelingInfo.size();

	m_canLevel = true;
	m_currentLevel = level;

	m_elementNum = m_pScript->AddEquip(icon, level);
	OnLevelUp();
}

OpticalImplant::~OpticalImplant()
{}

void OpticalImplant::OnAttach()
{}

void OpticalImplant::OnUpdate(float dt)
{}

void OpticalImplant::OnRender(float dt)
{}

void OpticalImplant::OnUnattach()
{}