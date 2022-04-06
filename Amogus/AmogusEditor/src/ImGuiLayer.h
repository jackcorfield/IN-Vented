#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../Fonts/IconHeader.h"

#include <Amogus.h>
#include "EntityInspectorGui.h"
#include "SceneHierarchyGui.h"

class ImGuiLayer
{
public:
	ImGuiLayer(Application* app);
	~ImGuiLayer();

	void BeginGui();
	void Draw();
	void EndGui();

	void DrawSceneView(int textureID);

	inline ImVec2 GetFrameSize() { return m_renderSize; };

	Framebuffer* m_gameView;
	bool m_sceneFrameResized = false;
private:
	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&) = delete;

	void DrawMenuBar();
	void DrawProfiler();
	void DrawConsole();

	void DrawNewEntityMenu();
	void SelectObject();

	void OnClick(InputEvent* e);

	Application* m_app;

	EntityInspectorGui m_entityInspector;
	SceneHierarchyGui m_sceneHierarchy;

	ImGuiDockNodeFlags m_dockspaceFlags;
	ImGuiWindowFlags m_windowFlags;

	bool m_guiEnabled = true;
	bool m_selecting; // Set to true by click event so we can do selection later (during ImGui phase)

	double m_mouseX, m_mouseY;

	ImVec2 m_renderSize;
	ImVec2 m_tempSize;

	EntityManager* m_entityManager;

	Entity m_selectedItem = 0;
};
