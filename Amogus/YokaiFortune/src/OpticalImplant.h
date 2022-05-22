#pragma once
#include "EquipmentScript.h"
class OpticalImplant :
    public EquipmentScript
{
    OpticalImplant(EntityManager* entityManager, Entity parentEntityID, Entity player, int level = 0);

};

