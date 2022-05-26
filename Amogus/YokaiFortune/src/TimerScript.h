#pragma once
#include "Resources/Script.h"
#include "Amogus.h" 
#include "PlayerScript.h"

class TimerScript :
    public Script
{
public:

    TimerScript(EntityManager* entityManager, Entity parentEntityID, Entity Timer, Entity playerID);
    ~TimerScript();

     void OnAttach() override;
     void OnUpdate(float dt) override;
     void OnRender(float dt) override;
     void OnUnattach() override;

     float m_seconds = 0.0f;
     PlayerScript* m_pScript;
};

