#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class WeaponScript :
    public Script
{
public:
    WeaponScript(EntityManager* entityManager, Entity parentEntityID, float speed);
    ~WeaponScript();
    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();

private:

    int m_maxLevel;

    BoxCollider m_boxCollider;
    Sprite  m_sprite;
    //Modifiers

    float m_hitDelay;

    float m_baseProjectileSpeed; //Speed of projectiles
    float m_baseProjectileCooldown; //How often weapon attacks
    float m_baseProjectileArea; //Size of weapon
    float m_baseProjectileDuration; //How long the projectile stays on the screen

    float m_baseProjectileCount; //How many projectiles
    float m_projectileMax;
    
    float m_baseDamageModifier;

    float m_critMultiplier;
};

