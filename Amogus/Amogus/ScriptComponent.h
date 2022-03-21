#pragma once

#include "EntityManager.h"
#include "Script.h"

class ScriptComponent
{
public:

	ScriptComponent(EntityManager* entityManager, Entity parentEntityID)
		: m_entityManager(entityManager),
		m_entityID(parentEntityID),
		m_attachedScript(NULL) {}
	~ScriptComponent() {}

	template <typename T>
	void AttachScript()
	{
		T* script = new T(m_entityManager, m_entityID);
		m_attachedScript = script;
	}

	Script* GetAttachedScript() const
	{
		return m_attachedScript;
	}

private:
	EntityManager* m_entityManager;
	Entity m_entityID;

	Script* m_attachedScript;
};