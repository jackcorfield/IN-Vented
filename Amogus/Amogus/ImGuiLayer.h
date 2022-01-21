#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Fonts/IconHeader.h"

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

private:

	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&) = delete;

	bool p_open;

	ImGuiDockNodeFlags m_dockspaceFlags;
	ImGuiWindowFlags m_windowFlags;

	ImGuiIO io;

	bool m_guiEnabled = true;

	float dragFloat[3] = {};
	char inputString[32] = {};
};

