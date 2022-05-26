#include "LevelingScreen.h"

LevelingScreen::LevelingScreen(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity kCount, std::vector<Entity> entities):// Entity startWeapon , std::vector<Entity*> weapons, std::vector<Entity*> equipment) :
	Script(entityManager, parentEntityID),
	m_leveling(parentEntityID),
	m_player(player),
	m_killCounter(kCount),
	m_Entities(entities),
	m_entityManager(entityManager)
{
	m_UIWidget = entityManager->GetComponent<UI_WidgetComponent>(parentEntityID);

	std::vector<UI_BaseElement*> elements = m_UIWidget->m_elements;

	button1 = (UI_ImageButton*)elements[0];//button
	button2 = (UI_ImageButton*)elements[1];//button
	button3 = (UI_ImageButton*)elements[2];//button

	std::vector<Sprite*> sprites;
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("NeonKatana", "Weapons/NeonKatana/neonkatana.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("LaserGun", "Weapons/LaserGun/LaserGun.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("Shuriken", "Weapons/Shuriken/Shuriken.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("Grenade", "Weapons/Grenade/grenade.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("HackingDevice", "Weapons/HackingDevice/HackingDevice.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("OpticalImplant", "Equipment/OpticalImplant.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("Passport", "Equipment/Passport.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("PowerGem", "Equipment/PowerGem.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("PowerGlove", "Equipment/PowerGlove.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));
	sprites.push_back(m_entityManager->AddComponent<Sprite>(m_leveling, TextureLoader::CreateTexture2DFromFile("Ragnite", "Equipment/Ragnite.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")));

	ItemData temp;
	for (int i = 0; i < 10; i++)
	{
		temp.ID = m_Entities[i];
		temp.sprite = sprites[i];
		temp.type = (ItemType)i;
		temp.active = false;
		temp.maxLevel = false;
		m_items.push_back(temp);
	}

	SetUpButtons();
}

LevelingScreen::~LevelingScreen()
{
}

void LevelingScreen::OnAttach()
{
}

void LevelingScreen::OnUpdate(float dt)
{
	if (button1->m_state != ButtonState::BS_Click)
		button1Click = false;

	if (button1->m_state == ButtonState::BS_Click && button1Click == false)
	{
		button1Click = true;

		if (m_items[m_itemNum].active == false)
			AddItem(&m_items[m_itemNum]);
		else
			LevelItem(&m_items[m_itemNum]);
	}
}

void LevelingScreen::OnRender(float dt)
{
}

void LevelingScreen::OnUnattach()
{
}

void LevelingScreen::AddItem(ItemData* item)
{
	ScriptComponent* scriptC = m_entityManager->GetComponent<ScriptComponent>(item->ID);
	
	if (!scriptC)
		return;

	if (item->active)
		return;

	switch (item->type)
	{
	case GRENADE:
		scriptC->AttachScript<Grenade>(m_player, item->ID, m_killCounter);
		break;
	case HACKING:
		scriptC->AttachScript<HackingDevice>(m_player, item->ID, m_killCounter);
		break;
	case LASER:
		scriptC->AttachScript<LaserGun>(m_player, item->ID, m_killCounter);
		break;
	case NEON:
		scriptC->AttachScript<NeonKatana>(m_player, item->ID, m_killCounter);
		break;
	case SHURIKEN:
		scriptC->AttachScript<Shuriken>(m_player, item->ID, m_killCounter);
		break;
	case IMPLANT:
		scriptC->AttachScript<OpticalImplant>(m_player, item->ID);
		break;
	case PASSPORT:
		scriptC->AttachScript<Passport>(m_player, item->ID);
		break;
	case GEM:
		scriptC->AttachScript<PowerGem>(m_player, item->ID);
		break;
	case GLOVE:
		scriptC->AttachScript<PowerGlove>(m_player, item->ID);
		break;
	case RAGNITE:
		scriptC->AttachScript<Ragnite>(m_player, item->ID);
		break;
	}

	item->active = true;
}

void LevelingScreen::LevelItem(ItemData* item)
{
	ScriptComponent* scriptC = m_entityManager->GetComponent<ScriptComponent>(item->ID);
	switch (item->type)
	{
	case GRENADE:
	{
		Grenade* grenade = (Grenade*)scriptC->GetAttachedScript();
		item->maxLevel = grenade->OnLevelUp();
		break;
	}
	case HACKING:
	{
		HackingDevice* hackingDevice = (HackingDevice*)scriptC->GetAttachedScript();
		item->maxLevel = hackingDevice->OnLevelUp();
		break;
	}
	case LASER:
	{
		LaserGun* laserGun = (LaserGun*)scriptC->GetAttachedScript();
		item->maxLevel = laserGun->OnLevelUp();
		break;
	}
	case NEON:
	{
		NeonKatana* neonKatana = (NeonKatana*)scriptC->GetAttachedScript();
		item->maxLevel = neonKatana->OnLevelUp();
		break;
	}
	case SHURIKEN:
	{
		Shuriken* shuriken = (Shuriken*)scriptC->GetAttachedScript();
		item->maxLevel = shuriken->OnLevelUp();
		break;
	}
	case IMPLANT:
	{
		OpticalImplant* implant = (OpticalImplant*)scriptC->GetAttachedScript();
		item->maxLevel = implant->OnLevelUp();
		break;
	}
	case PASSPORT:
	{
		Passport* passport = (Passport*)scriptC->GetAttachedScript();
		item->maxLevel = passport->OnLevelUp();
		break;
	}
	case GEM:
	{
		PowerGem* gem = (PowerGem*)scriptC->GetAttachedScript();
		item->maxLevel = gem->OnLevelUp();
		break;
	}
	case GLOVE:
	{
		PowerGlove* glove = (PowerGlove*)scriptC->GetAttachedScript();
		item->maxLevel = glove->OnLevelUp();
		break;
	}
	case RAGNITE:
	{

		Ragnite* ragnite = (Ragnite*)scriptC->GetAttachedScript();
		item->maxLevel = ragnite->OnLevelUp();
		break;
	}
	}
}

void LevelingScreen::SetUpButtons()
{
	std::vector<UI_BaseElement*> elements = m_UIWidget->m_elements;

	UI_Image* image1 = (UI_Image* )elements[3];//image
	image1->m_texture = m_items[0].sprite->GetTexture();

	button2 = (UI_ImageButton*)elements[1];//button
	UI_Image* image2 = (UI_Image*)elements[4];//image
	image2->m_texture = m_items[2].sprite->GetTexture();

	button3 = (UI_ImageButton*)elements[2];//button
	UI_Image* image3 = (UI_Image*)elements[5];//image
	image3->m_texture = m_items[7].sprite->GetTexture();
}
