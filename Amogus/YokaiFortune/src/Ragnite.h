#pragma once
#include "EquipmentScript.h"
class Ragnite :
    public EquipmentScript
{
public:
    Ragnite(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level = 0);
};

