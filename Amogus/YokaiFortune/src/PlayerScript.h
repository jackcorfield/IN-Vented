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
    
    struct KeyRegister
    {
        bool W, A, S, D;
    };
    
    KeyRegister m_registeredKeys;

    Entity m_player;
    EntityManager* m_manager;
    float m_speed;

    bool m_movePlayer;
    int m_latestKeyInput;
    int m_latestGameInput;


    void KeyEvent(InputEvent* e);


};

