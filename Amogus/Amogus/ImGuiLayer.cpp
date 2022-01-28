#include "ImGuiLayer.h"

#include "source.h"
#include <GLFW/glfw3.h>
#include <string.h>

#include "Shader.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Audio.h"
#include "PlayerMovement.h"
#include "Physics.h"
#include "BoxCollider.h"

#include <iostream>

#include "SceneExporter.h"
#include "SceneImporter.h"

extern Application* g_app;

ImGuiLayer::ImGuiLayer()
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


	m_selectedBool = false;
	m_gameView = new Framebuffer;

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

void ImGuiLayer::DrawMenuBar(Shader* shader)
{
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem(ICON_FA_FILE"  New"))
				{
					//Do something
				}
				else if (ImGui::MenuItem(ICON_FA_FILE_IMPORT"	Import"))
				{
					SceneImporter::ImportSceneFromFile("testimport.json");
				}
				else if (ImGui::MenuItem(ICON_FA_FILE_EXPORT"	Export"))
				{
					SceneExporter::ExportActiveSceneToFile(g_app->m_sceneManager->GetActiveSceneName() + ".json");
				}
				ImGui::EndMenu();

			}

			if (ImGui::BeginMenu("Assets"))
			{
				if (ImGui::BeginMenu("Add Component"))
				{
					if (ImGui::MenuItem("Sprite"))
					{
						Entity entity = m_entityManager->CreateEntity();
						Sprite* sprite = m_entityManager->AddComponent<Sprite>(entity, TextureLoader::CreateTexture2DFromFile("defaultEntity", "test.png"), glm::vec3(1.0f, 1.0f, 1.0f), shader);
						Transform* testTransform = m_entityManager->AddComponent<Transform>(entity, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f);
					}
					else if (ImGui::MenuItem("Camera"))
					{
						Entity entity = m_entityManager->CreateEntity();
						Camera* camera = m_entityManager->AddComponent<Camera>(entity, g_app->m_windowParams.windowWidth, g_app->m_windowParams.windowHeight, -1.0f, 1.0f, new Framebuffer);
						Transform* cameraTransform = m_entityManager->AddComponent<Transform>(entity, glm::vec2(50.0f, 100.0f), glm::vec2(0.0f));
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem(ICON_FA_COG" Settings"))
			{


			}

			ImGui::EndMainMenuBar();
		}
	}
}

void ImGuiLayer::DrawHierachy()
{
	
	ImGui::Begin("Hierachy");

	ImGui::InputText("Search", inputString, sizeof(inputString));

	std::vector<Entity> allEntities = m_entityManager->GetAllActiveEntities();

	static uint32_t item_current_idx = 0;

	for (uint32_t i = 0; i < allEntities.size(); i++)
	{

		
		const bool is_selected = (item_current_idx == i);
		std::string selectLabel = "Entity " + std::to_string(i);

		if (ImGui::TreeNode(selectLabel.c_str()))
		{
			std::map<std::type_index, void*> entityComponents = m_entityManager->GetAllComponents(allEntities[i]);

			for (const auto& component : entityComponents)
			{
				//std::cout << typeid(component.first).name() << std::endl;
				if (component.first == std::type_index(typeid(Sprite)))
				{
					selectLabel = "Sprite##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(AnimatedSprite)))
				{
					selectLabel = "Animated Sprite##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(Camera)))
				{
					selectLabel = "Camera##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(Audio)))
				{
					selectLabel = "Audio##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(Transform)))
				{
					selectLabel = "Transform##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(Physics)))
				{
					selectLabel = "Physics##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(PlayerMovement)))
				{
					selectLabel = "Player Movement##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else if (component.first == std::type_index(typeid(BoxCollider)))
				{
					selectLabel = "Box Coliider##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
				else
				{
					selectLabel = "Default##" + std::to_string(i);
					if (ImGui::TreeNode(selectLabel.c_str()))
					{
						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
	
	}

	ImGui::End();
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

void ImGuiLayer::DrawInspector()
{



	ImGui::Begin("Inspector");
	ImGui::InputText("Component Name", inputString, 32);

	if (m_selectedItem != 0)
	{
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::DragFloat3("Position", dragFloat, 0.1f, 0.1f, 0.1f, "%.3f");
			ImGui::DragFloat3("Rotation", dragFloat, 0.1f, 0.1f, 0.1f, "%.3f");
			ImGui::DragFloat3("Scale", dragFloat, 0.1f, 0.1f, 0.1f, "%.3f");
		}
		if (ImGui::CollapsingHeader("Sprite Renderer"))
		{
			ImGui::Text("Add some fun sprite stuff here idk");
		}
	}
	


	ImGui::End();
}

void ImGuiLayer::DrawSceneView(int textureID)
{
	ImGui::Begin("Scene View");
	//Render the scene here.
	
	m_tempSize = ImGui::GetContentRegionAvail();
	if (m_tempSize.x != m_renderSize.x || m_renderSize.y != m_tempSize.y)
	{
		m_sceneFrameResized = true;
		m_renderSize.x = m_tempSize.x;
		m_renderSize.y = m_tempSize.y;
	}
	
	ImGui::Image(ImTextureID(textureID), m_renderSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	
	ImGui::Begin("Game View");
	ImGui::Image(ImTextureID(m_gameView->GetRenderTextureID()), m_renderSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}