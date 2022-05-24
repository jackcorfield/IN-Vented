#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

#include <vector>
#include <string>

static const std::vector<std::string> weaponSlotNames
{
    "WeaponSlot1",
    "WeaponSlot2",
    "WeaponSlot3",
    "WeaponSlot4",
    "WeaponSlot5"
};

const std::vector<std::string> equipSlotNames
{
    "EquipSlot1",
    "EquipSlot2",
    "EquipSlot3",
    "EquipSlot4",
    "EquipSlot5"
};


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

    void AddWeapon(Sprite* icon);
    void AddEquip(Sprite* icon);

    void AddXP(int XPVal);

    // INITIAL WEAPON
    
    UI_WidgetComponent* m_UIWidget;

    int m_weaponCount = 0;
    int m_equipCount = 5; //needs to be 5 higher than the weapon slots

   //Modifiers
    float m_movementSpeed;

    int m_projectileSpeed; //Speed of projectiles (Percentage increase)
    int m_projectileCount; //How many projectiles 
    int m_projectileCooldown; //How often weapon attacks (Percentage of reduction)
    int m_projectileArea; //Size of weapon (Percentage increase)
    int m_projectileDuration; //How long the projectile stays on the screen (Percentage increase)

    float m_damageModifier;

    float m_maxHealth;
    float m_health;
    float m_regeneration;

    float m_range; //Pick up distance

private:
    void UpdateSpriteAnimation(bool facingLeft, bool moving);
    
    int m_playerXP;

    // Enemy collision functions
    void CheckCollisions();
    bool CheckPotentialCollision(Entity possibleCollision); // Simple AABB collision check
    glm::vec2 GetIntersectionDepth(Entity collidedEntity);
    void ResolveCollision(glm::vec2 intersection, BoxCollider* theirCollider, Transform* theirTransform);

    // Stores player components for quick access
    Transform* m_transform;
    BoxCollider* m_collider;
    
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

    float m_invulnTime; // Stores length of time player is invulnerable after taking damage (prevents instant death from being hit every frame)
    float m_currentInvulnCooldown; // Counts down to 0 from m_invulnTime after taking damage
    bool m_isDead;

    void KeyEvent(InputEvent* e);
};

