#pragma once
#include "EquipmentScript.h"
class PowerGlove :
    public EquipmentScript
{
public:
    PowerGlove(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level = 0);
};

