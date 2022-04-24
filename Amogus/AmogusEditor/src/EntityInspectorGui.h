#pragma once

#include <Amogus.h>

#include <functional>

// Holds data for and draws gui listing the components of a given entity, allowing for runtime editing
class EntityInspectorGui
{
public:
	EntityInspectorGui();

	void Draw();

	void SetActiveEntity(Entity entity);

private:
	void CreateAddComponentGui();

	Entity m_activeEntity;
};