#pragma once
#include "WeaponScript.h"


class HackingDevice : public WeaponScript
{
public:
    HackingDevice(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level = 1, bool moving = false, bool autoTarget = false);
    ~HackingDevice();

    void OnAttach();
    void OnUpdate(float dt);
    void OnRender(float dt);
    void OnUnattach();
    void SpawnProjectile();

private:
    glm::vec2 m_originalTransformSize, m_originalBoxSize, m_originalBoxOffset;


};
