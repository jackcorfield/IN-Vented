#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class PlayerScript :
    public Script
{
public:
    PlayerScript(EntityManager* entityManager, Entity parentEntityID, float speed);
    ~PlayerScript();

    virtual void OnAttach() override;
    virtual void OnUpdate(float dt) override;
    virtual void OnRender(float dt) override;
    virtual void OnUnattach() override;

private:
    void UpdateSpriteAnimation(bool facingLeft, bool moving);
    
    bool m_keyboardInUse; // Prevents dpad input if keyboard is already being used in the frame
    struct KeyRegister // Stores status of relevant keys
    {
        bool W, A, S, D;
        bool UP, DOWN, LEFT, RIGHT;
    } m_registeredKeys;
    //int m_latestKeyInput;

    bool m_leftStickInUse; // True when left stick has been used in the frame
    glm::vec2 m_leftStickDirection;
    //int m_latestGameInput;

    bool m_movingLastFrame; // Used to test against current movement status (to set correct animation)
    bool m_facingLeftLastFrame; // Used to determine whether to flip sprite

    //Entity m_player; If all we are using this for is to check if player exists, it is probably not worth keeping - if player doesn't exist, this script doesn't run
    //EntityManager* m_manager;

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

    void KeyEvent(InputEvent* e);
};

