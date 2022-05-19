#pragma once

#include <ECS/EntityManager.h>
#include <string>

class Script
{
public:
	Script(EntityManager* entityManager, Entity parentEntityID)
		: m_entityManager(entityManager),
		m_entityID(parentEntityID) {}
	virtual ~Script() {}

	virtual void OnAttach() {}
	virtual void OnUpdate(float dt) {}
	virtual void OnRender(float dt) {}
	virtual void OnUnattach() {}

protected:

	// ECS
	template <typename T, class ...ARGS>
	T* AddComponent(ARGS&&... params)
	{
		m_entityManager->AddComponent<T>(m_entityID, std::forward<ARGS>(params));
	}

	template <typename T>
	bool HasComponent()
	{
		return m_entityManager->HasComponent<T>(m_entityID);
	}

	template <typename T>
	T* GetComponent()
	{
		return m_entityManager->GetComponent<T>(m_entityID);
	}

	template <typename T>
	bool RemoveComponent()
	{
		return m_entityManager->RemoveComponent<T>(m_entityID);
	}

	// Scenes
	bool SceneExists(const std::string& name);
	void GotoScene(const std::string& name);
	std::string GetCurrentScene() const;

	// Collision
	bool IsColliding(Entity entity, bool precise = true);

private:
	Entity m_entityID;
	EntityManager* m_entityManager;
};