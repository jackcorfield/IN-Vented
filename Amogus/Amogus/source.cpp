#include "source.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Fonts/IconHeader.h"

#include "EntityManager.h"

void error_callback(int error, const char* description);
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

Application::Application() :
	m_entityManager(nullptr),
	m_renderer(nullptr)
{	
}

struct NameComponent
{
	NameComponent(const std::string& _name)
		: Name(_name)
	{}

	std::string Name;
};

void Application::Init()
{
	m_windowParams =
	{
		1920, 1080,
		false,
		0,
		"Engine"
	};

	InitGL();
	InitImGui();

	m_entityManager = new EntityManager();
	m_renderer = new Renderer();

	Run();
}

void Application::Run()
{
	
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		m_renderer->Render();
		
	}
	}

	

Application::~Application() 
{
	delete m_renderer;
	m_renderer = nullptr;
}

bool Application::InitGL()
{
	// Initialise glfw
	if (!glfwInit())
	{
		std::cerr << "Failed to initialise GLFW!" << std::endl;
		return false;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* monitor = m_windowParams.isFullscreen ? glfwGetPrimaryMonitor() : NULL; // If fullscreen enabled, get monitor
	m_window = glfwCreateWindow(m_windowParams.windowWidth, m_windowParams.windowHeight, m_windowParams.windowTitle, monitor, NULL);
	if (!m_window)
	{
		std::cerr << "Failed to create window!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(m_window);

	// Set glfw callback(s)
	glfwSetFramebufferSizeCallback(m_window, frame_buffer_size_callback);

	// Prevents window from closing instantly
	glfwSetWindowShouldClose(m_window, GL_FALSE);

		// Initialise glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialise glad!" << std::endl;
		return false;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (m_windowParams.MSAASamples > 0)
		glEnable(GL_MULTISAMPLE);
}

void Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
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
}

void Application::TerminateOpenGL()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	glfwTerminate();
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}