#pragma once
#include "WeaponScript.h"

class Shuriken :
	public WeaponScript
{
    public:
	Shuriken(EntityManager* entityManager, Entity parentEntityID, Entity player, Sprite icon, Sprite sprite, glm::vec2 hitboxSize, int level = 0, bool moving = true, bool autoTarget = false);
    ~Shuriken();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void SpawnProjectile();

private:


};

