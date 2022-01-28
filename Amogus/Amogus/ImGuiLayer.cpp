#include "ImGuiLayer.h"

#include "source.h"
#include <GLFW/glfw3.h>

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

	m_gameView = new Framebuffer;
}

ImGuiLayer::~ImGuiLayer()
{

}

void ImGuiLayer::BeginGui()
{
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

void ImGuiLayer::DrawMenuBar()
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

					}
					else if (ImGui::MenuItem("Camera"))
					{

					}
					else if (ImGui::MenuItem("Tile Map"))
					{

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
	if (ImGui::TreeNode("Scene1"))
	{
		if (ImGui::TreeNode("Sprite1"))
		{
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Sprite2"))
		{
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera1"))
		{
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("TileMap"))
		{
			if (ImGui::TreeNode("Tile1"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile2"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile3"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile4"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile5"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile6"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile7"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile8"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Tile9"))
			{
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
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
	
	ImGui::Image(ImTextureID(2), m_renderSize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	
	ImGui::Begin("Game View");
	ImGui::Image(ImTextureID(m_gameView->GetRenderTextureID()), m_renderSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}