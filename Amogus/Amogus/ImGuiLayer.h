#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Fonts/IconHeader.h"
#include "Framebuffer.h"

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void BeginGui();
	void EndGui();

	void DrawMenuBar();
	void DrawHierachy();
	void DrawProfiler();
	void DrawConsole();
	void DrawInspector();
	void DrawSceneView();

	inline ImVec2 GetFrameSize() { return m_renderSize; };

	Framebuffer* m_gameView;
	bool m_sceneFrameResized = false;
private:

	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&) = delete;

	ImGuiDockNodeFlags m_dockspaceFlags;
	ImGuiWindowFlags m_windowFlags;

	bool m_guiEnabled = true;

	float dragFloat[3] = {};
	char inputString[32] = {};

	ImVec2 m_renderSize;
	ImVec2 m_tempSize;
};

