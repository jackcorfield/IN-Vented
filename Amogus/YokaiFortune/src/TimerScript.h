#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class TimerScript :
    public Script
{
public:

    TimerScript(EntityManager* entityManager, Entity parentEntityID, Entity Timer);
    ~TimerScript();

     void OnAttach() override;
     void OnUpdate(float dt) override;
     void OnRender(float dt) override;
     void OnUnattach() override;

     float m_seconds = 0.0f;
};

