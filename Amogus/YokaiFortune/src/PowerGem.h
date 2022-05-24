#pragma once
#include "EquipmentScript.h"
class PowerGem :
    public EquipmentScript
{
public:
    PowerGem(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level = 0);
};

