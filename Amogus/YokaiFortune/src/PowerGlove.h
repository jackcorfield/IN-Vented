#pragma once
#include "EquipmentScript.h"
class PowerGlove :
    public EquipmentScript
{
    PowerGlove(EntityManager* entityManager, Entity parentEntityID, Entity player, int level = 0);
};

