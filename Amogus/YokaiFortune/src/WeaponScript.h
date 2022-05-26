#pragma once
#include "Resources/Script.h"
#include "Amogus.h"
#include "PlayerScript.h"
#include "EnemySpawnerScript.h"
#include "XpManagerScript.h"

class WeaponScript :
    public Script
{
public:
    WeaponScript(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, Entity killCounter, int level = 1, bool moving = true, bool autoTarget = false);
    ~WeaponScript();
    void SetSprites(Sprite* icon, Sprite* sprite);
    void OnAttach();
    virtual void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();

    bool OnLevelUp();
    virtual void SpawnProjectile();

protected:

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
        bool isSpawned;
    };

    bool CheckWeaponCollision(Entity weaponID, bool areaOfEffect = false);

    Entity m_weapon;
    Entity m_player;
    Entity e_xpManager;
    Entity m_killCounter;
    EntityManager* m_manager;

    int m_killCount;
    int m_elementNum;
    bool m_firstLevel;

   Sprite* m_icon;
   Sprite* m_sprite;
   Audio* m_audio;
   PlayerScript* m_pScript;
   EnemySpawnerScript* m_eScript;
   XpManager* m_xpManager;

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

    int m_currentProjectile;

    glm::vec2 m_playerPreviousPosition;

    std::vector<std::pair<modifier, int>> m_levelingInfo; // WILL NEED TO BE SET UP FOR EACH WEAPON

    glm::vec2 m_originalTransformSize, m_originalBoxSize, m_originalBoxOffset;
};

