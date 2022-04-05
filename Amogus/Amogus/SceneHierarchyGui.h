#pragma once

#include "Scene.h"

class EntityInspectorGui;

// Holds data for and draws gui of entity hierarchy of a given scene
class SceneHierarchyGui
{
public:
	SceneHierarchyGui() = default;

	void Draw();

	void SetCurrentScene(Scene* scene);
	void SetEntityInspector(EntityInspectorGui* entityInspector);

private:
	// Return true if clicked
	bool DrawEntityNode(Entity entity, int i);
	//bool DrawComponentNode(void* component, std::type_index type, int i);

	Scene* m_activeScene;
	Entity m_selectedEntity;
	EntityInspectorGui* m_entityInspector;
};