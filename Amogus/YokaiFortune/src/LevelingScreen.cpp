#include "LevelingScreen.h"

LevelingScreen::LevelingScreen(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity kCount, std::vector<Entity> entities):
	Script(entityManager, parentEntityID),
	m_leveling(parentEntityID),
	m_player(player),
	m_killCounter(kCount),
	m_Entities(entities),
	m_entityManager(entityManager)
{
	m_pScript = (PlayerScript*)entityManager->GetComponent<ScriptComponent>(player)->GetAttachedScript();

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

	m_items[2].active = true;

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

	for (int i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].maxLevel)
			m_items.erase(m_items.begin()+i, m_items.begin() + i+1);
	}

	if (m_pScript->m_needLevel)
	{
		SetUpButtons();
		m_pScript->m_needLevel = false;
	}
		

	if (m_UIWidget->m_elements[0]->m_hidden)
		return;

	if (m_items.size() == 0)
	{
		for (UI_BaseElement* elements : m_UIWidget->m_elements)
			elements->m_hidden = true;

		return; // do some other stuff
	}

	if (button1->m_state != ButtonState::BS_Click)
		button1Click = false;

	if (button1->m_state == ButtonState::BS_Click && button1Click == false)
	{
		button1Click = true;

		if (m_items[button1Num].active == false)
			AddItem(&m_items[button1Num]);
		else
			LevelItem(&m_items[button1Num]);

		for (UI_BaseElement* element : m_UIWidget->m_elements)
			element->m_hidden = true;
	}

	if (button2->m_state != ButtonState::BS_Click)
		button2Click = false;

	if (button2->m_state == ButtonState::BS_Click && button2Click == false)
	{
		button2Click = true;

		if (m_items[button2Num].active == false)
			AddItem(&m_items[button2Num]);
		else
			LevelItem(&m_items[button2Num]);

		for (UI_BaseElement* element : m_UIWidget->m_elements)
			element->m_hidden = true;
	}

	if (button3->m_state != ButtonState::BS_Click)
		button3Click = false;

	if (button3->m_state == ButtonState::BS_Click && button3Click == false)
	{
		button3Click = true;

		if (m_items[button3Num].active == false)
			AddItem(&m_items[button3Num]);
		else
			LevelItem(&m_items[button3Num]);

		for (UI_BaseElement* element : m_UIWidget->m_elements)
			element->m_hidden = true;
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
	for (UI_BaseElement* element : m_UIWidget->m_elements)
		element->m_hidden = false;

	srand((unsigned)time(NULL));

	int random1;
	int random2;
	int random3;

	if (m_items.size() >= 3)
	{
		random1 = 0 + (rand() % m_items.size());
		random2 = 0 + (rand() % m_items.size());
		random3 = 0 + (rand() % m_items.size());
	}
	if (m_items.size() == 2)
	{
		random1 = 0;
		random2 = 1;


	}
	if (m_items.size() == 1)
	{
		random1 = 0;
	}

	std::vector<UI_BaseElement*> elements = m_UIWidget->m_elements;
	UI_Image* image1 = (UI_Image*)elements[3];//image
	if (m_items.size() >= 1)
	{
		image1->m_texture = m_items[random1].sprite->GetTexture();
		button1Num = random1;
	}
	else
	{
		button1->m_hidden = true;
		image1->m_hidden = true;
	}

	UI_Image* image2 = (UI_Image*)elements[4];//image
	if (m_items.size() >= 2)
	{
		button2 = (UI_ImageButton*)elements[1];//button
		image2->m_texture = m_items[random2].sprite->GetTexture();
		button2Num = random2;
	}
	else
	{
		button2->m_hidden = true;
		image2->m_hidden = true;
	}

	UI_Image* image3 = (UI_Image*)elements[5];//image
	if (m_items.size() >= 3)
	{
		button3 = (UI_ImageButton*)elements[2];//button
		image3->m_texture = m_items[random3].sprite->GetTexture();
		button3Num = random3;
	}
	else
	{
		button3->m_hidden = true;
		image3->m_hidden = true;
	}

}
