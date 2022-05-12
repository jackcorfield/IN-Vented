#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class WeaponScript :
    public Script
{
public:
    WeaponScript(EntityManager* entityManager, Entity parentEntityID, int level);
    ~WeaponScript();
    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();

    void OnLevelUp();

private:
    enum modifier
    {
        SPEED = 0,
        COOLDOWN,
        AREA,
        DURATION,
        COUNT,
        DAMAGE
    };

    Entity m_player;
    EntityManager* m_manager;

    int m_maxLevel;

    bool m_canLevel;
    int m_currentLevel;

    //Modifiers

    int m_baseProjectileSpeed; //Speed of projectiles
    int m_baseProjectileCooldown; //How often weapon attacks
    int m_baseProjectileArea; //Size of weapon
    int m_baseProjectileDuration; //How long the projectile stays on the screen
    int m_baseProjectileCount; //How many projectiles
    int m_projectileMax;
    
    int m_baseDamageModifier;

    float m_hitDelay;
    float m_critMultiplier;

    std::vector<std::pair<modifier, int>> m_levelingInfo; // WILL NEED TO BE SET UP FOR EACH WEAPON
};

