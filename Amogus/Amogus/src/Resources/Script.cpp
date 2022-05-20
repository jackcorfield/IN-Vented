#include "Script.h"

#include "../Core/source.h"

extern Application* g_app;

bool Script::SceneExists(const std::string& name)
{
	return g_app->m_sceneManager->SceneExists(name);
}

void Script::GotoScene(const std::string& name)
{
	g_app->m_sceneManager->SetActiveScene(name);
}

std::string Script::GetCurrentScene() const
{
	return g_app->m_sceneManager->GetActiveSceneName();
}

bool Script::IsColliding(Entity entity, bool precise)
{
	if (precise)
		return g_app->m_collisionManager->checkCollision(m_entityID, entity);
	
	std::vector<Entity> potentialCollisions = g_app->m_collisionManager->potentialCollisions(m_entityID);
	
	return (std::find(potentialCollisions.begin(), potentialCollisions.end(), entity) != potentialCollisions.end());
}

std::vector<Entity> Script::GetNearbyEntities() const
{
	return g_app->m_collisionManager->potentialCollisions(m_entityID);
}
