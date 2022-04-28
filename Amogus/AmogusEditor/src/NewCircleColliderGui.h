#pragma once

#include "IGuiObject.h"

#include "Amogus.h"

struct NewCircleColliderGui :
	public IGuiObject
{
public:
	NewCircleColliderGui(Entity entityFor);

	virtual void CreateGui() override;

private:
	void AddCircleCollider();

	Entity entity;

	float inputRadius;
	glm::vec2 inputCentrePos;
};