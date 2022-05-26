#pragma once
#include "Resources\Script.h"
#include "Amogus.h"
#include "WeaponScript.h"
#include "EquipmentScript.h"

#include "Grenade.h"
#include "HackingDevice.h"
#include "LaserGun.h"
#include "NeonKatana.h"
#include "Shuriken.h"

#include "OpticalImplant.h"
#include "Passport.h"
#include "PowerGem.h"
#include "PowerGlove.h"
#include "Ragnite.h"

class LevelingScreen :
    public Script
{

public:
	LevelingScreen(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity kCount, std::vector<Entity> weapons);//, Entity startWeapon, std::vector<Entity*> weapons, std::vector<Entity*> equipment);
	~LevelingScreen();

	void OnAttach() override;
	void OnUpdate(float dt) override;
	void OnRender(float dt) override;
	void OnUnattach() override;

	enum ItemType
	{
		NEON = 0,
		LASER,
		SHURIKEN,
		GRENADE,
		HACKING,
		IMPLANT,
		PASSPORT,
		GEM,
		GLOVE,
		RAGNITE
	};

	struct ItemData
	{
		Entity		ID;
		Sprite* sprite;
		ItemType	type;
		bool			active;
		bool			maxLevel;
	};

	void AddItem(ItemData* item);
	void LevelItem(ItemData* item);
	void SetUpButtons();

	bool button1Click = false;
	bool button2Click = false;
	bool button3Click = false;

	int m_itemNum = 0;

	PlayerScript* m_pScript;

	std::vector<ItemData*> selectedItems;

	UI_ImageButton* button1;
	UI_ImageButton* button2;
	UI_ImageButton* button3;

	int button1Num;
	int button2Num;
	int button3Num;

	Entity m_leveling;
	Entity m_player;
	Entity m_killCounter;
	EntityManager* m_entityManager;
	std::vector<Entity> m_Entities;

	UI_WidgetComponent* m_UIWidget;

	std::vector<ItemData>	m_items;
	std::vector<Entity*> m_maxLevelItems;
};

