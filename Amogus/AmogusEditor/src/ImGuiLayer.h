#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../Fonts/IconHeader.h"

#include <Amogus.h>
#include "GuiObjects/EntityInspectorGui.h"
#include "GuiObjects/SceneHierarchyGui.h"

#define STYLE_LIGHT_MODE 0
#define STYLE_DARK_MODE 1

class ImGuiLayer
{
public:
	ImGuiLayer(Application* app);
	~ImGuiLayer();

	void BeginGui();
	void Draw();
	void EndGui();

	void DrawSceneView(int textureID);

	void ChangeFont(const std::string& fontPath, const float sizeInPixels);
	void ChangeStyle();
	void SetConsoleColours(const ImVec4& debugColour, const ImVec4& warningColour, const ImVec4& errorColour);

	inline ImVec2 GetFrameSize() { return m_renderSize; };
	float GetFontSize() const { return m_fontSize; }
	std::string GetFontPath() const { return m_fontPath; }
	ImVec4 GetDebugColour() const { return m_debugColour; }
	ImVec4 GetWarningColour() const { return m_warningColour; }
	ImVec4 GetErrorColour() const { return m_errorColour; }
	bool GetColourMode() const { return m_colourMode; }

	Framebuffer* m_gameView;
	bool m_sceneFrameResized = false;

	struct Game
	{
		std::string Name;
		std::vector<std::string> Scenes;

		std::string CurrentSceneName;
	};

	Game game;

	bool m_roundedCorners;

private:
	ImGuiLayer(const ImGuiLayer&) = delete;
	ImGuiLayer& operator=(ImGuiLayer&) = delete;

	void DrawMenuBar();
	void DrawProfiler();
	void DrawConsole();
	void DrawPlayPauseStopButton(); // Draws a play that controls whether the game is running in-editor, a pause button to pause it, and a stop button to stop runtime

	void CreateGame(char* name);
	void SaveGame();
	void LoadGame(char* name);

	void CreateScene(char* name, float col[3]);
	void SaveScene();
	bool LoadScene(const char* name);

	void DrawNewEntityMenu();
	void SelectObject();

	void LoadSettingsFromFile();
	void SaveSettingsToFile();

	void OnClick(InputEvent* e);

	Application* m_app;

	EntityInspectorGui m_entityInspector;
	SceneHierarchyGui m_sceneHierarchy;

	std::vector<std::unique_ptr<IGuiObject>> m_guiObjects; // Stores pointers to dialog boxes
	std::unique_ptr<IGuiObject> m_popup; // Stores pointer to popup object
	std::list<std::string> m_recentScenes; // Stores names of recently imported scenes for easy access

	ImGuiDockNodeFlags m_dockspaceFlags;
	ImGuiWindowFlags m_windowFlags;

	bool m_guiEnabled = true;
	bool m_selecting; // Set to true by click event so we can do selection later (during ImGui phase)
	bool m_colourMode; // Light == 0, Dark == 1

	bool m_sceneLoaded = false;
	bool m_gameLoaded = false;
	

	double m_mouseX, m_mouseY;

	ImVec2 m_renderSize;
	ImVec2 m_tempSize;
	ImVec2 m_menuBarSize;

	EntityManager* m_entityManager;

	std::string m_windowTitle;

private:
	friend class NewGameGui;
	friend class LoadGameGui;
	friend class CreateSceneGui;
	friend class ImportSceneGui;

	// Popups
	bool m_showGameNotLoadedErrorState = false;

	bool m_showNewGameDialogState = false;
	bool m_showLoadGameDialogState = false;

	int m_numOfConsoleEntries;
	bool m_scrollToBottom;
	bool m_autoScroll;

	bool m_fontDirty;
	float m_fontSize;
	std::string m_fontPath;
	ImVec4 m_debugColour, m_warningColour, m_errorColour;
};
