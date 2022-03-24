#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../Fonts/IconHeader.h"

#include <Amogus.h>

class ImGuiLayer
{
public:
	ImGuiLayer(Application* app);
	~ImGuiLayer();

	void BeginGui();
	void EndGui();

	void DrawMenuBar();
	void DrawHierarchy();
	void DrawProfiler();
	void DrawConsole();
	void DrawInspector();
	void DrawSceneView(int textureID);

	inline ImVec2 GetFrameSize() { return m_renderSize; };

	Framebuffer* m_gameView;
	bool m_sceneFrameResized = false;
private:
	Application* m_app;

	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&) = delete;

	ImGuiDockNodeFlags m_dockspaceFlags;
	ImGuiWindowFlags m_windowFlags;

	bool m_guiEnabled = true;

	float dragFloat[3] = {};
	char inputString[32] = {};
	bool m_selectedBool;

	ImVec2 m_renderSize;
	ImVec2 m_tempSize;

	EntityManager* m_entityManager;

	Entity m_selectedItem = 0;
};
