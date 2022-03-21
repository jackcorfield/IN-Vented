#include "ScriptSystem.h"

#include "source.h"

extern Application* g_app;

void ScriptSystem::OnUpdate(float dt)
{
	Scene* currentScene = g_app->m_sceneManager->GetActiveScene();
	if (!currentScene)
		return;

	std::vector<ScriptComponent*> scriptComponents = currentScene->m_entityManager->GetAllComponentsOfType<ScriptComponent>();

	for (ScriptComponent* s : scriptComponents)
	{
		if (s->GetAttachedScript() != NULL)
		{
			s->GetAttachedScript()->OnUpdate(dt);
		}
	}
}

void ScriptSystem::OnRender(float dt)
{
	Scene* currentScene = g_app->m_sceneManager->GetActiveScene();
	if (!currentScene)
		return;

	std::vector<ScriptComponent*> scriptComponents = currentScene->m_entityManager->GetAllComponentsOfType<ScriptComponent>();

	for (ScriptComponent* s : scriptComponents)
	{
		if (s->GetAttachedScript() != NULL)
		{
			s->GetAttachedScript()->OnRender(dt);
		}
	}
}
