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
	m_sceneManager(nullptr),
	m_renderer(nullptr),
	m_physicsSystem(nullptr),
	m_quit(false)
{	
	InputHandler::GetMapping("Input_Exit")->m_bus->subscribe(this, &Application::Quit);
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
		1920, 1030,
		false,
		0,
		"Engine"
	};

	InitGL();

	m_entityManager = new EntityManager();
	m_renderer = new Renderer();

	Run();
}

void Application::Run()
{

	EngineUtils::Timer* Timer = EngineUtils::Timer::Instance();

	// Locked to 60fps for now, will change at later date
	float frameRate = 60.0f;

	while (!m_quit)
	{
		Timer->Tick();
		if (Timer->DeltaTime() >= 1 / frameRate) {

			Timer->Reset();
			//std::cout << Timer->DeltaTime() << std::endl;

			glfwPollEvents();
			m_physicsSystem->PhysicsUpdate(0.66);
			m_renderer->Render();

		}
	}

	TerminateOpenGL();
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

void Application::Quit(KeyInputEvent* e)
{
	m_quit = true;
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}