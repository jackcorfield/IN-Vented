#include "ImGuiLayer.h"

#include <Amogus.h>

#include <GLFW/glfw3.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include <nlohmann/include/nlohmann/json.hpp>
#include "GuiObjects/DialogBoxes/NewGameGui.h"
#include "GuiObjects/DialogBoxes/LoadGameGui.h"
#include "GuiObjects/DialogBoxes/CreateSceneGui.h"
#include "GuiObjects/DialogBoxes/ImportSceneGui.h"
#include "GuiObjects/DialogBoxes/ErrorDialogGui.h"

#define MAX_RECENT_SCENES 5

extern Application* g_app;

void AddNameToUniqueQueueList(std::list<std::string>* list, std::string name, const unsigned int maxItems);

ImGuiLayer::ImGuiLayer(Application* app) :
	m_app(app),
	m_selecting(false),
	m_mouseX(0),
	m_mouseY(0),
	m_guiEnabled(true),
	m_windowFlags(0),
	m_dockspaceFlags(0),
	m_entityManager(nullptr)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(g_app->m_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	style.ChildRounding = 12;
	style.FrameRounding = 12;
	style.GrabRounding = 12;
	style.PopupRounding = 12;
	style.ScrollbarRounding = 12;
	style.TabRounding = 12;
	style.WindowRounding = 12;
	style.ChildBorderSize = 0;
	style.FrameBorderSize = 0;
	style.PopupBorderSize = 0;
	style.TabBorderSize = 0;
	style.WindowBorderSize = 0;

	io.Fonts->AddFontFromFileTTF("Fonts/ComicNeueAngular-Regular.ttf", 16);

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("Fonts/FontIcons.ttf", 16.0f, &config, icon_ranges);

	InputHandler::m_mouseClickBus->subscribe(this, &ImGuiLayer::OnClick);

	m_gameView = new Framebuffer;

	m_sceneHierarchy.SetEntityInspector(&m_entityInspector);

	m_windowTitle = "Amogus Editor";
	glfwSetWindowTitle(g_app->m_window, m_windowTitle.c_str());
}

ImGuiLayer::~ImGuiLayer()
{
	if (m_popup) { m_popup.release(); }
}

void ImGuiLayer::BeginGui()
{
	if (g_app->m_sceneManager->GetActiveScene() != NULL)
		m_entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	static bool p_open = true;

	// AP - ImGui rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("MainDockSpace", &p_open, window_flags);

	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();

	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}

void ImGuiLayer::Draw()
{

	DrawMenuBar();
	m_sceneHierarchy.SetCurrentScene(g_app->m_sceneManager->GetActiveScene());
	m_sceneHierarchy.Draw();
	DrawConsole();
	m_entityInspector.Draw();
	DrawProfiler();
	
	if (m_popup)
	{
		m_popup.get()->CreateGui();
		if (m_popup.get()->close)
		{
			m_popup.release();
		}
	}
}

void ImGuiLayer::EndGui()
{
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup_current_context);
}

void ImGuiLayer::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(ICON_FA_FILE"  New Game"))
			{
				m_popup = std::make_unique<NewGameGui>(this);
			}
			else if (ImGui::MenuItem(ICON_FA_FILE_EXPORT"   Save Game"))
			{
				if (m_gameLoaded)
					SaveGame();
				else
					m_popup = std::make_unique<ErrorDialogGui>("No game loaded");
			}
			else if (ImGui::MenuItem(ICON_FA_FILE_IMPORT"   Load Game"))
			{
				m_popup = std::make_unique<LoadGameGui>(this);
			}
			ImGui::Separator();

			if (ImGui::MenuItem(ICON_FA_PLUS"	Create Scene"))
			{
				if (m_gameLoaded)
					m_popup = std::make_unique<CreateSceneGui>(this);
				else
					m_popup = std::make_unique<ErrorDialogGui>("No game loaded");
			}

			if (ImGui::MenuItem(ICON_FA_FILE_EXPORT"	Save Scene"))
			{
				if (m_sceneLoaded)
					SaveScene();
				else
					m_popup = std::make_unique<ErrorDialogGui>("No scene loaded");
			}

			if (ImGui::MenuItem(ICON_FA_FILE_IMPORT"	Load Scene"))
			{
				m_popup = std::make_unique<ImportSceneGui>(this, &m_recentScenes);
			}

			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_FILE_EXPORT"	Export and Run"))
			{
				if (m_sceneLoaded)
					SaveScene();
				if (m_gameLoaded)
				{
					SaveGame();
					std::string command = "YokaiFortune.exe -game " + game.Name;
					system(command.c_str());
				}
				else
					m_popup = std::make_unique<ErrorDialogGui>("No game loaded");
			}

			ImGui::Separator();
			
			if (ImGui::BeginMenu("Import recent scene"))
			{
				if (m_gameLoaded)
				{
					int i = 0; // Predeclare iterator so it can be pre-incremented

					std::string toAdd = "";
					for (std::string name : m_recentScenes)
					{
						if (ImGui::MenuItem(name.c_str()))
						{
							if (LoadScene(name.c_str()))
								toAdd = name;
						}

						i++; // Increment for each slot used
					}

					if (!toAdd.empty())
					{
						AddNameToUniqueQueueList(&m_recentScenes, toAdd, MAX_RECENT_SCENES);
					}

					// Display any remaining slots (up to max) using pre-incremented i
					for (i; i < MAX_RECENT_SCENES; i++)
					{
						if (ImGui::MenuItem("-")) {}
					}
				}
				
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem(ICON_FA_CROSSHAIRS"   Exit"))
			{
				g_app->Quit();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Assets"))
		{
			if (m_sceneLoaded)
				DrawNewEntityMenu();

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem(ICON_FA_COG" Settings"))
		{

		}

		m_menuBarSize = ImGui::GetWindowSize();
		DrawPlayPauseStopButton();

		ImGui::EndMainMenuBar();
	}
}

void ImGuiLayer::DrawNewEntityMenu()
{
	if (ImGui::BeginMenu("New entity"))
	{
		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

		if (ImGui::MenuItem("Empty"))
		{
			Entity entity = entityManager->CreateEntity();
			m_entityInspector.SetActiveEntity(entity);
		}

		if (ImGui::MenuItem("GameObject"))
		{
			Entity entity = entityManager->CreateEntity();
			m_entityInspector.SetActiveEntity(entity);

			Sprite* sprite = entityManager->AddComponent<Sprite>(entity, TextureLoader::CreateTexture2DFromFile("defaultEntity", "test.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
			Transform* transform = entityManager->AddComponent<Transform>(entity, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f);
			EntityName* name = entityManager->AddComponent<EntityName>(entity, "New GameObject");
		}

		if (ImGui::MenuItem("Camera"))
		{
			Entity entity = entityManager->CreateEntity();
			m_entityInspector.SetActiveEntity(entity);

			Camera* camera = entityManager->AddComponent<Camera>(entity, g_app->m_windowParams.windowWidth, g_app->m_windowParams.windowHeight, -1.0f, 1.0f, new Framebuffer());
			Transform* transform = entityManager->AddComponent<Transform>(entity, glm::vec2(50.0f, 100.0f), glm::vec2(0.0f));
			EntityName* name = entityManager->AddComponent<EntityName>(entity, "New camera");
		}

		ImGui::EndMenu();
	}
}

void ImGuiLayer::DrawProfiler()
{
	ImGui::Begin("Profiler");
	static float arr[] = { 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f };
	ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr), 0, "", 0, 30, ImVec2(ImGui::GetWindowWidth() - 100, 30));

	ImGui::End();

	
}

void ImGuiLayer::DrawConsole()
{
	ImGui::Begin("Console");
	ImGui::End();	
}

void ImGuiLayer::CreateGame(char* name)
{
	if (!m_gameLoaded)
	{
		m_gameLoaded = true;
		game.Name = name;
		game.Scenes = {};

		SaveGame();

		m_windowTitle = "Game: " + game.Name + " Scene: " + game.CurrentSceneName + " - Amogus Editor";
		glfwSetWindowTitle(g_app->m_window, m_windowTitle.c_str());
	}
}

void ImGuiLayer::SaveGame()
{
	if (m_gameLoaded)
	{
		std::string path = "Data/Config/" + game.Name + ".json";
		std::ofstream outFile(path);

		nlohmann::json json;
		json["name"] = game.Name;
		json["scenes"] = game.Scenes;

		outFile << json;
		outFile.close();
	}
}

void ImGuiLayer::LoadGame(char* name)
{
	std::string path = "Data/Config/" + std::string(name) + ".json";
	std::ifstream inFile(path);
	if (inFile.is_open())
	{
		if (m_gameLoaded)
		{
			game.Name = "";
			game.CurrentSceneName = "";
			game.Scenes = {};
		}
		
		nlohmann::json json;
		inFile >> json;
		game.Name = json["name"];
		game.Scenes = json["scenes"].get<std::vector<std::string>>();

		//bool sceneLoaded = SceneImporter::ImportSceneFromFile(std::string(game.Scenes[0]) + ".json", true);
		
		bool sceneLoaded = false;
		if (game.Scenes.size() > 0)
		{
			sceneLoaded = SceneImporter::ImportSceneFromFile(std::string(game.Scenes[0]), true);
		}
		
		m_gameLoaded = true;
		if (sceneLoaded)
		{
			m_sceneLoaded = true;
			game.CurrentSceneName = game.Scenes[0];
		}

		m_windowTitle = "Game: " + game.Name + " Scene: " + game.CurrentSceneName + " - Amogus Editor";
		glfwSetWindowTitle(g_app->m_window, m_windowTitle.c_str());
		
		inFile.close();
	}
}

void ImGuiLayer::CreateScene(char* name, float col[3])
{
	if (std::find(game.Scenes.begin(), game.Scenes.end(), std::string(name)) == game.Scenes.end())
	{
		g_app->m_sceneManager->CreateScene(std::string(name), glm::vec3(col[0], col[1], col[2]));
		g_app->m_sceneManager->SetActiveScene(name);
		Scene* activeScene = g_app->m_sceneManager->GetActiveScene();

		Entity entity = activeScene->m_entityManager->CreateEntity();

		Camera* camera = activeScene->m_entityManager->AddComponent<Camera>(entity, g_app->m_windowParams.windowWidth, g_app->m_windowParams.windowHeight, -1.0f, 1.0f, new Framebuffer());
		Transform* transform = activeScene->m_entityManager->AddComponent<Transform>(entity, glm::vec2(50.0f, 100.0f), glm::vec2(0.0f));
		EntityName* nameComponent = activeScene->m_entityManager->AddComponent<EntityName>(entity, "Scene Camera");

		g_app->SetActiveCamera(entity);
		
		SceneExporter::ExportActiveSceneToFile(std::string(name));
		game.Scenes.push_back(name);
		game.CurrentSceneName = name;
		m_sceneLoaded = true;

		m_windowTitle = "Game: " + game.Name + " Scene: " + game.CurrentSceneName + " - Amogus Editor";
		glfwSetWindowTitle(g_app->m_window, m_windowTitle.c_str());
	}
}

void ImGuiLayer::SaveScene()
{
	SceneExporter::ExportActiveSceneToFile(std::string(game.CurrentSceneName));	
}

bool ImGuiLayer::LoadScene(const char* name)
{
	if (g_app->m_sceneManager->SceneExists(name))
	{
		g_app->m_sceneManager->SetActiveScene(name);
		return true;
	}
	bool success = SceneImporter::ImportSceneFromFile(name, true);
	if (success)
	{
		if (std::find(game.Scenes.begin(), game.Scenes.end(), name) == game.Scenes.end())
		{
			game.Scenes.push_back(name);
		}

		m_sceneLoaded = true;
		game.CurrentSceneName = name;

		m_windowTitle = "Game: " + game.Name + " Scene: " + game.CurrentSceneName + " - Amogus Editor";
		glfwSetWindowTitle(g_app->m_window, m_windowTitle.c_str());
	}
	return success;
}

void ImGuiLayer::DrawPlayPauseStopButton()
{
	bool paused = g_app->IsPaused();

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 buttonPos(io.DisplaySize.x / 2.0f, m_menuBarSize.y / 4.0f);
	ImGui::SetCursorPos(buttonPos);

	const char* buttonLabel = "";
	if (paused)
	{
		buttonLabel = ICON_FA_PLAY;
		if (ImGui::Button(buttonLabel))
		{
			SceneExporter::ExportActiveSceneToFile("temp");
			g_app->SetPause(false);
		}
	}
	else
	{
		buttonLabel = ICON_FA_PAUSE;
		if (ImGui::Button(buttonLabel))
		{
			g_app->SetPause(true);
		}

		buttonLabel = ICON_FA_STOP;
		if (ImGui::Button(buttonLabel))
		{
			g_app->SetPause(true);

			std::string activeSceneName = g_app->m_sceneManager->GetActiveSceneName();
			g_app->m_sceneManager->DestroyScene(activeSceneName);
			SceneImporter::ImportSceneFromFile("temp", true);
			m_sceneHierarchy.SetCurrentScene(g_app->m_sceneManager->GetActiveScene());
		}
	}
}

void ImGuiLayer::DrawSceneView(int textureID)
{
	ImGui::Begin("Scene View");
	//Render the scene here.

	
	if (m_selecting)
	{
		//SelectObject(); selection by click is currently disabled due to it's unfinished state and time constraints
		m_selecting = false;
	}

	ImVec2 tempSize = ImGui::GetContentRegionAvail();
	if (tempSize.x != m_renderSize.x || m_renderSize.y != tempSize.y)
	{
		m_sceneFrameResized = true;
		m_renderSize.x = tempSize.x;
		m_renderSize.y = tempSize.y;
	}
	
	ImGui::Image(ImTextureID(textureID), m_renderSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	
	ImGui::Begin("Game View");
	ImGui::Image(ImTextureID(m_gameView->GetRenderTextureID()), m_renderSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void ImGuiLayer::SelectObject()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Get mouse pos in world space
	{
		ImVec2 windowPosIm = ImGui::GetCursorScreenPos();
		ImVec2 mousePosIm = ImGui::GetMousePos();
		glm::vec2 windowPos = glm::vec2(windowPosIm.x, windowPosIm.y);
		glm::vec2 mousePos = glm::vec2(mousePosIm.x, mousePosIm.y);

		// Check that click is in window
		if (mousePos.x > windowPos.x + g_app->m_windowParams.windowWidth)
			return;
		if (mousePos.x < windowPos.x)
			return;
		if (mousePos.y > windowPos.y + g_app->m_windowParams.windowHeight)
			return;
		if (mousePos.y < windowPos.y)
			return;

		Camera* activeCamera = nullptr;
		{
			std::vector<Camera*> allCameras = entityManager->GetAllComponentsOfType<Camera>();
			for (Camera* camera : allCameras)
			{
				if (camera->m_isActive)
				{
					activeCamera = camera;
					break;
				}
			}
		}

		Entity cameraEntity = entityManager->GetEntityFromComponent<Camera>(activeCamera);
		Transform* cameraTransform = entityManager->GetComponent<Transform>(cameraEntity);

		glm::vec2 localMousePos = mousePos - windowPos;

		if (localMousePos.x > ImGui::GetWindowSize().x || localMousePos.y > ImGui::GetWindowSize().y) // Outside window bounds
		{
			return;
		}

		glm::vec2 worldMousePos = localMousePos + cameraTransform->m_position;

		m_mouseX = worldMousePos.x;
		m_mouseY = worldMousePos.y;
	}

	Entity selectedEntity = 0;

	std::vector<Transform*> allTransforms = entityManager->GetAllComponentsOfType<Transform>();
	for (Transform* transform : allTransforms)
	{
		Entity entity = entityManager->GetEntityFromComponent<Transform>(transform);
		glm::vec2 pos = transform->m_position;
		glm::vec2 size = transform->m_size;

		if (entityManager->HasComponent<Sprite>(entity))
		{
			Sprite* sprite = entityManager->GetComponent<Sprite>(entity);
			Texture2D texture = sprite->GetTexture();
			size.x = texture.m_width;
			size.y = texture.m_height;
		}
		else if (entityManager->HasComponent<AnimatedSprite>(entity))
		{
			AnimatedSprite* sprite = entityManager->GetComponent<AnimatedSprite>(entity);
			Texture2D firstFrame = sprite->GetFrames()[0];
			size.x = firstFrame.m_width;
			size.y = firstFrame.m_height;
		}
		else
		{
			size *= 200.0f;
		}

		if (m_mouseX > pos.x + size.x)
			continue;
		if (m_mouseX < pos.x)
			continue;
		if (m_mouseY > pos.y + size.y)
			continue;
		if (m_mouseY < pos.y)
			continue;

		selectedEntity = entity;
	}

	m_entityInspector.SetActiveEntity(selectedEntity);
}

void ImGuiLayer::OnClick(InputEvent* e)
{
	if (e->m_action == GLFW_PRESS)
	{
		m_selecting = true; // We can't do object selection in event because it won't be during ImGui phase
	}
}

void AddNameToUniqueQueueList(std::list<std::string>* list, std::string name, const unsigned int maxItems)
{
	list->erase(std::remove(list->begin(), list->end(), name), list->end()); // Remove possible previous instance of this name (thus moving it to the front)
	list->push_front(name);
	if (list->size() > maxItems)
	{
		list->pop_back();
	}
}