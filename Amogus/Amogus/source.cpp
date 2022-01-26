#include "source.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>

#include "EntityManager.h"
#include "Timer.h"

void error_callback(int error, const char* description);
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void window_close_callback(GLFWwindow* window);

extern Application* g_app;

Application::Application() :
	m_entityManager(nullptr),
	m_sceneManager(nullptr),
	m_renderer(nullptr),
	m_audioManager(nullptr),
	m_collisionManager(nullptr),
	m_quit(false)
{	
	InputHandler::GetMapping("Input_Exit")->m_bus->subscribe(this, &Application::Quit);
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


	m_entityManager = new EntityManager();
	m_sceneManager = new SceneManager();
	m_audioManager = new AudioManager();
	m_collisionManager = new CollisionManager();

	m_sceneManager->CreateScene("Main Scene", glm::vec3(0.2f, 0.3f, 0.8f));

	m_renderer = new Renderer();

	InputHandler();

	Run();
}

void Application::Run()
{

	EngineUtils::Timer* Timer = EngineUtils::Timer::Instance();
	bool isRunning = true;
	// Locked to infinity for now, will change at later date
	constexpr float frameRate = std::numeric_limits<float>::infinity();

	while (!m_quit)
	{
    Timer->Tick();
		if (Timer->DeltaTime() >= 1 / frameRate)
		{
			Timer->Reset();
			glfwPollEvents();
			PhysicsSystem::Update(Timer->DeltaTime());
			m_collisionManager->CheckCollision();
			m_renderer->Render(Timer->DeltaTime());
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
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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

	// Set input callback(s)
	glfwSetKeyCallback(m_window, InputHandler::KeyCallback);
	glfwSetCursorPosCallback(m_window, InputHandler::MouseCallback);
	glfwSetMouseButtonCallback(m_window, InputHandler::MouseButtonCallback);

	glfwSetWindowCloseCallback(m_window, window_close_callback);

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

void Application::Quit(KeyInputEvent* e)
{
	m_quit = true;
}

void Application::Quit()
{
	m_quit = true;
}

void Application::TerminateOpenGL()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	glfwTerminate();
	InputHandler::Cleanup();
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void window_close_callback(GLFWwindow* window)
{
	g_app->Quit();
}

