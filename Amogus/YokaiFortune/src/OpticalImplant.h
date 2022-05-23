#pragma once
#include "EquipmentScript.h"

class OpticalImplant :
    public EquipmentScript
{
public:
    OpticalImplant(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level = 0);
    ~OpticalImplant();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
};

