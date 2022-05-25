#pragma once

#include "Resources/Script.h"
#include "PlayerScript.h"

#include "../Amogus.h"

class XpManager :
    public Script
{
    struct Orb
    {
        Entity      id;
        bool        isSpawned;
        int         xpVal;
    };

public:
	XpManager(EntityManager* entityManager, Entity parentEntityID, Entity player, int maxOrbs, int poolingSize);

	virtual void OnAttach() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender(float dt) override;
	virtual void OnUnattach() override;

    void SpawnOrb(glm::vec2 position, int XpVal, bool isRecurse = false);

private:
	int m_maxOrbs;
	int m_poolingSize;
    int m_maximumPoolable;
    Entity m_player;

    EntityManager* m_manager;
    PlayerScript* m_playerScript;

    

    std::vector<Orb> m_orbs;

    Orb* FindOrb(Entity orbID);

};