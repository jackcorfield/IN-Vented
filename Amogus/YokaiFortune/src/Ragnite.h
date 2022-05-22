#pragma once
#include "EquipmentScript.h"
class Ragnite :
    public EquipmentScript
{
    Ragnite(EntityManager* entityManager, Entity parentEntityID, Entity player, int level = 0);
};

