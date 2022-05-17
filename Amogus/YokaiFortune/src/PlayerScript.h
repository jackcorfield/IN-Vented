#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class PlayerScript :
    public Script
{
public:
    PlayerScript(EntityManager* entityManager, Entity parentEntityID, float speed);
    ~PlayerScript();
    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();

private:
    
    bool m_keyboardInUse;
    struct KeyRegister
    {
        bool W, A, S, D;
        bool UP, DOWN, LEFT, RIGHT;
    };
    
    KeyRegister m_registeredKeys;

    bool m_leftStickInUse;
    glm::vec2 m_leftStickDirection;

    Entity m_player;
    EntityManager* m_manager;

    // INITIAL WEAPON

    //Modifiers
    float m_movementSpeed;

    float m_projectileSpeed; //Speed of projectiles
    float m_projectileCount; //How many projectiles
    float m_projectileCooldown; //How often weapon attacks
    float m_projectileArea; //Size of weapon
    float m_projectileDuration; //How long the projectile stays on the screen

    float m_damageModifier;

    float m_health;
    float m_regeneration;

    float m_range; //Pick up distance

    bool m_movePlayer;

    int m_latestKeyInput;
    int m_latestGameInput;


    void KeyEvent(InputEvent* e);


};

