#include "ImGuiLayer.h"

#include <Amogus.h>

#include <GLFW/glfw3.h>
#include <string.h>
#include <iostream>

#include "GuiObjects/DialogBoxes/CreateSceneGui.h"
#include "GuiObjects/DialogBoxes/ExportSceneGui.h"
#include "GuiObjects/DialogBoxes/ImportSceneGui.h"

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

}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::BeginGui()
{
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

	//ImGui::ShowDemoWindow();

	if (m_popup)
	{
		m_popup.get()->CreateGui();
		if (m_popup.get()->close)
		{
			m_popup.release();
		}
	}

	//// Draw each dialog box
	//for (int i = 0; i < m_guiObjects.size(); i++)
	//{
	//	IGuiObject* guiObject = m_guiObjects[i].get();

	//	guiObject->CreateGui();

	//	if (guiObject->close) // If this window is ready to close, delete object
	//	{
	//		m_guiObjects.erase(m_guiObjects.begin() + i);
	//	}
	//}
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
			if (ImGui::MenuItem(ICON_FA_FILE"  New"))
			{
				m_popup = std::make_unique<CreateSceneGui>();
			}
			else if (ImGui::MenuItem(ICON_FA_FILE_IMPORT"	Import"))
			{
				m_popup = std::make_unique<ImportSceneGui>(&m_recentScenes);
			}
			else if (ImGui::MenuItem(ICON_FA_FILE_EXPORT"	Export"))
			{
				m_popup = std::make_unique<ExportSceneGui>();
			}
			else if (ImGui::BeginMenu("Import recent"))
			{
				int i = 0; // Predeclare iterator so it can be pre-incremented

				std::string toAdd = "";
				for (std::string name : m_recentScenes)
				{
					if (ImGui::MenuItem(name.c_str()))
					{
						std::string activeSceneName = g_app->m_sceneManager->GetActiveSceneName();

						bool success = SceneImporter::ImportSceneFromFile(name, true);
						if (success)
						{
							g_app->m_sceneManager->DestroyScene(activeSceneName);
						}
					
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

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Assets"))
		{
			DrawNewEntityMenu();

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem(ICON_FA_COG" Settings"))
		{

		}

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