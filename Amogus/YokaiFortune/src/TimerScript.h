#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class TimerScript :
    public Script
{
    TimerScript(EntityManager* entityManager, Entity parentEntityID);
    ~TimerScript();

     void OnAttach() override;
     void OnUpdate(float dt) override;
     void OnRender(float dt) override;
     void OnUnattach() override;

     int m_seconds = 0;
     int m_minutes = 0;
};

