#pragma once
#include "EquipmentScript.h"
class Passport :
    public EquipmentScript
{
public:
    Passport(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity equipment, int level = 0);
};

