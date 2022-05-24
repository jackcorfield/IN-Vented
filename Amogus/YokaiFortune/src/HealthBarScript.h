#pragma once

#include "Resources/Script.h"

#include "../Amogus.h"

class HealthBarScript :
	public Script
{
public:
	HealthBarScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID);

	virtual void OnAttach() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender(float dt) override;
	virtual void OnUnattach() override;

private:
	Entity m_trackedPlayer; // Player entity to display health of
	const int m_maxImageWidth = 50.0f;
};

