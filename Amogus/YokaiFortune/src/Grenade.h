#pragma once
#include "WeaponScript.h"

class Grenade :
	public WeaponScript
{
    public:
        Grenade(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level = 0, bool moving = true, bool autoTarget = false);
    ~Grenade();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void SpawnProjectile();

private:


};

