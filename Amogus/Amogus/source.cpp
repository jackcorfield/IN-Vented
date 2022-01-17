#include "source.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

void error_callback(int error, const char* description);
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

Application::Application() :
	m_renderer(nullptr)
{	
}

void Application::Init()
{
	m_windowParams =
	{
		1280, 720,
		false,
		0,
		"Engine"
	};

	InitGL();
	InitImGui();

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
	glfwSetErrorCallback(error_callback);

	// Initialise glfw
	if (!glfwInit())
	{
		std::cerr << "Failed to initialise GLFW!" << std::endl;
		return false;
	}

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (m_windowParams.MSAASamples > 0)
		glEnable(GL_MULTISAMPLE);
}

void Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
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