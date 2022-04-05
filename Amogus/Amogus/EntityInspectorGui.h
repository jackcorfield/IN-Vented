#pragma once

#include "EntityManager.h"

// Holds data for and draws gui listing the components of a given entity, allowing for runtime editing
class EntityInspectorGui
{
public:
	EntityInspectorGui() = default;

	void Draw();

	void SetActiveEntity(Entity entity);

private:
	Entity m_activeEntity;
};