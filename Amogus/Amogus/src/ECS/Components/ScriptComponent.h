#pragma once

#include <ECS/EntityManager.h>
#include <Resources/Script.h>

class ScriptComponent
{
public:

	ScriptComponent(EntityManager* entityManager, Entity parentEntityID)
		: m_entityManager(entityManager),
		m_entityID(parentEntityID),
		m_attachedScript(NULL) {}
	~ScriptComponent() {}

	template <typename T, class ...ARGS>
	void AttachScript(ARGS&&... params)
	{
		if (m_attachedScript != NULL)
		{
			m_attachedScript->OnUnattach();
		}
		T* script = new T(m_entityManager, m_entityID, std::forward<ARGS>(params)...);
		m_attachedScript = script;
		m_attachedScript->OnAttach();
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