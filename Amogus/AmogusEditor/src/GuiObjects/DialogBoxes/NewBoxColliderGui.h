#pragma once

#include "../IGuiObject.h"

#include "Amogus.h"

struct NewBoxColliderGui :
	public IGuiObject
{
public:
	NewBoxColliderGui(Entity entityFor);

	virtual void CreateGui() override;

private:
	void AddBoxCollider();

	Entity entity;

	glm::vec2 inputSize;
};