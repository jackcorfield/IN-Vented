#include <Amogus.h>

#include "ImGuiLayer.h"

class AmogusEditor : public Application
{
public:

	void onInit() override
	{
		m_gui = new ImGuiLayer(this);
		m_renderer->setFramebuffer(m_gui->m_gameView);

		g_app->SetPause(true); // Pause runtime on launch
	}

	void onUpdate(float dt) override
	{

	}

	void onRender(float dt) override
	{

	}

	void onImGui() override
	{
		m_gui->BeginGui();
		
		m_gui->Draw();

		// If there is an active camera, draw scene view
		{
			EntityManager* entityManager = m_sceneManager->GetActiveScene()->m_entityManager;
			Entity cameraEntity = m_renderer->GetSceneCamera();

			if (entityManager->IsEntity(cameraEntity) && entityManager->HasComponent<Camera>(cameraEntity))
			{
				Camera* cameraComponent = entityManager->GetComponent<Camera>(m_renderer->GetSceneCamera());
				m_gui->DrawSceneView(cameraComponent->m_framebuffer->GetRenderTextureID());
			}
		}

		m_gui->EndGui();
	}

	ImGuiLayer* m_gui;

private:
};

Application* CreateApplication()
{
	return new AmogusEditor();
}