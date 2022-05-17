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
