#pragma once

#pragma once

#include "Resources/Script.h"

#include "../Amogus.h"

class CameraFollowScript :
	public Script
{
public:
	CameraFollowScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID = 0);

	virtual void OnAttach() override;
	virtual void OnUpdate(float dt) override;

private:
	// Tries to find an entity called "Player" to follow
	Entity FindPlayer();

	Entity m_followTarget; // Player entity to follow
	Camera* m_camera; // Store camera component to access internal viewport
	Transform* m_transform; // Store transform to modify position
};