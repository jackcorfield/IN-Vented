#pragma once
#include "WeaponScript.h"

class Grenade :
	public WeaponScript
{
    public:
        Grenade(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level = 1, bool moving = true, bool autoTarget = false);
    ~Grenade();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void SpawnProjectile();

private:

    struct Projectiles
    {
        Entity        name;
        float           duration;
        glm::vec2   direction;
        bool isSpawned;
        glm::vec2 originPos;
        float distance;
        bool hasHitTheFloor;
    };

    std::vector<Projectiles> m_vecProjectiles;

    Transform tempEnemy;

};

