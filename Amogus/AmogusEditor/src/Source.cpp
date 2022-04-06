#include <Amogus.h>

#include "ImGuiLayer.h"

class AmogusEditor : public Application
{
public:

	void onInit() override
	{
		m_gui = new ImGuiLayer(this);
		m_renderer->setFramebuffer(m_gui->m_gameView);
	}

	void onUpdate(float dt) override
	{

	}

	void onRender(float dt) override
	{

	}

	void onImGui() override
	{
		Scene* activeScene = m_sceneManager->GetActiveScene();
		Camera* cameraComponent = activeScene->m_entityManager->GetComponent<Camera>(m_renderer->GetSceneCamera());
		m_gui->BeginGui();
		m_gui->Draw();
		m_gui->DrawSceneView(cameraComponent->m_framebuffer->GetRenderTextureID());
		m_gui->EndGui();
	}

	ImGuiLayer* m_gui;


private:
};

Application* CreateApplication()
{
	return new AmogusEditor();
}