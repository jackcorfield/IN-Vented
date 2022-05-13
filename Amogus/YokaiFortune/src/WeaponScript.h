#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class WeaponScript :
    public Script
{
public:
    WeaponScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Sprite icon, Sprite sprite, glm::vec2 hitboxSize , int level = 0, bool moving = true, bool autoTarget = false);
    ~WeaponScript();
    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();

    void OnLevelUp();
    void SpawnProjectile();

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

    struct Projectiles
    {
        Entity        name;
        float           duration;
        glm::vec2   direction;
    };

    Entity m_weapon;
    Entity m_player;
    EntityManager* m_manager;

   Sprite m_icon;
   Sprite m_sprite;

    glm::vec2 m_hitboxSize;

    //std::vector<std::pair<Entity, float>> m_vecProjectiles;

    std::vector<Projectiles> m_vecProjectiles;


    bool m_isMoving;
    bool m_isAutoTarget;

    int m_maxLevel;

    bool m_canLevel;
    int m_currentLevel;

    float m_currentCooldown;
    //Modifiers

    float m_baseProjectileSpeed; //Speed of projectiles
    float m_baseProjectileCooldown; //How often weapon attacks
    float m_baseProjectileArea; //Size of weapon
    float m_baseProjectileDuration; //How long the projectile stays on the screen
    int m_baseProjectileCount; //How many projectiles
    int m_projectileMax;
    
    float m_wait;

    float m_baseDamageModifier;

    float m_hitDelay;
    float m_critMultiplier;

    bool m_isMax;

    glm::vec2 m_playerPreviousPosition;

    std::vector<std::pair<modifier, int>> m_levelingInfo; // WILL NEED TO BE SET UP FOR EACH WEAPON
};

