#include <Amogus.h>

#include "ImGuiLayer.h"

class AmogusEditor : public Application
{
public:

	void onInit() override
	{
		g_app->m_debugger->Log("Launched editor.", LL_DEBUG);
		glfwSetWindowTitle(m_window, "Amogus Editor");

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

		Scene* activeScene = m_sceneManager->GetActiveScene();
		if (activeScene)
		{
			Camera* cameraComponent = activeScene->m_entityManager->GetComponent<Camera>(m_renderer->GetSceneCamera());
			if (cameraComponent)
			{

				m_gui->DrawSceneView(cameraComponent->m_framebuffer->GetRenderTextureID());
			}
		}
		m_gui->EndGui();
	}

	virtual void onQuit() override
	{
		delete m_gui;
		m_gui = nullptr;
	}

	ImGuiLayer* m_gui;

private:
};

Application* CreateApplication(int argc, char** argv)
{
	return new AmogusEditor();
}