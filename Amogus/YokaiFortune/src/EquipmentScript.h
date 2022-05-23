#pragma once
#include "Resources/Script.h"
#include "Amogus.h"
#include "PlayerScript.h"

class EquipmentScript : 
    Script
{
public:

    EquipmentScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level = 0);
    ~EquipmentScript();

    void SetSprites(Sprite* icon);
    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void OnLevelUp();

    enum modifier
    {
        SPEED = 0,
        COOLDOWN,
        AREA,
        DURATION,
        COUNT
    };

    Entity m_equipment;
    Entity m_player;
    EntityManager* m_manager;

    Sprite* m_icon;//set in header of each equipment

    int m_maxLevel; //set in header of each equipment
    int m_currentLevel;

    bool m_canLevel;

    std::vector<std::pair<modifier, int>> m_levelingInfo; //set in header of each equipment

protected:
    PlayerScript* m_pScript;
};

