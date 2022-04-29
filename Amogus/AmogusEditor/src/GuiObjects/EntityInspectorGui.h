#pragma once

#include <Amogus.h>
#include <memory>

#include "IGuiObject.h"

// Holds data for and draws gui listing the components of a given entity, allowing for runtime editing
class EntityInspectorGui
{
public:
	EntityInspectorGui();
	~EntityInspectorGui();

	void Draw();

	void SetActiveEntity(Entity entity);

private:
	void DrawInspectorInfo(); // Separated so we can return early and not break ImGui's Begin/End rules
	void CreateAddComponentGui();

	std::vector<std::unique_ptr<IGuiObject>> m_guiObjects; // Stores pointers to dialog boxes (for components without default constructors)
	Entity m_activeEntity;
};