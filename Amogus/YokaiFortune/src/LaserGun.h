#pragma once
#include "WeaponScript.h"
class LaserGun :
    public WeaponScript
{
public:
    LaserGun(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level = 0, bool moving = true, bool autoTarget = false);
    ~LaserGun();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void SpawnProjectile(int currentPos);

private:


};

