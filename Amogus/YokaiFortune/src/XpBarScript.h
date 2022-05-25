#pragma once

#include "Resources/Script.h"

#include "../Amogus.h"

class XpBarScript :
	public Script
{
public:
	XpBarScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID);

	virtual void OnAttach() override;
	virtual void OnUpdate(float dt) override;

private:
	Entity m_trackedPlayer; // Player entity to display health of
	const int m_maxImageWidth = 500.0f;
};