#pragma once

#include "Resources/Script.h"

#include "../Amogus.h"

class TileLoop : public Script {

public:
	TileLoop(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID = 0);

	virtual void OnAttach() override;
	void ShiftTexture(float amount);
};