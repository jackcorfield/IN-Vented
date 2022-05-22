#pragma once
#include "EquipmentScript.h"
class PowerGem :
    public EquipmentScript
{
    PowerGem(EntityManager* entityManager, Entity parentEntityID, Entity player, int level = 0);
};

