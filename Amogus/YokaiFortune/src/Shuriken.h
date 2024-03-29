#pragma once
#include "WeaponScript.h"

class Shuriken :
	public WeaponScript
{
    public:
	Shuriken(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, Entity m_killCounter, int level = 0, bool moving = true, bool autoTarget = false);
    ~Shuriken();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void SpawnProjectile();

private:
    glm::vec2 m_previousDirection;

};

