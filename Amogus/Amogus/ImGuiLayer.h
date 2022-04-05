#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Fonts/IconHeader.h"
#include "Framebuffer.h"

#include "EntityManager.h"

#include "EntityInspectorGui.h"
#include "SceneHierarchyGui.h"

class InputEvent;
class Shader;

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void BeginGui();
	void Draw();
	void EndGui();

	void DrawMenuBar(Shader* shader);
	void DrawSceneView(int textureID);

	inline ImVec2 GetFrameSize() { return m_renderSize; };

	Framebuffer* m_gameView;
	bool m_sceneFrameResized = false;

private:
	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&) = delete;

	void DrawProfiler();
	void DrawConsole();


	void DrawNewEntityMenu();
	void SelectObject();

	void OnClick(InputEvent* e);

	EntityInspectorGui m_entityInspector;
	SceneHierarchyGui m_sceneHierarchy;

	ImGuiDockNodeFlags m_dockspaceFlags;
	ImGuiWindowFlags m_windowFlags;

	bool m_guiEnabled;
	bool m_selecting; // Set to true by click event so we can do selection later (during ImGui phase)
	double m_mouseX, m_mouseY;

	ImVec2 m_renderSize;

	EntityManager* m_entityManager;
};