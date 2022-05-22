#pragma once
#include "EquipmentScript.h"
class Passport :
    public EquipmentScript
{
    Passport(EntityManager* entityManager, Entity parentEntityID, Entity player, int level = 0);
};

