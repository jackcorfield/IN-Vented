#include "source.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>
#include <fstream>

#include <ECS/EntityManager.h>
#include "Timer.h"

#include <Scenes/SceneImporter.h>
#include <Scenes/SceneExporter.h>

void error_callback(int error, const char* description);
void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void window_close_callback(GLFWwindow* window);

extern Application* g_app;

Application::Application() :
	m_entityManager(nullptr),
	m_sceneManager(nullptr),
	m_renderer(nullptr),
	m_audioManager(nullptr),
	m_scriptSystem(nullptr),
	m_debugger(nullptr),
	m_collisionManager(nullptr),
	m_window(nullptr),
	m_windowParams({}),
	m_quit(false),
	m_pauseRuntime(false)
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
	m_scriptSystem = new ScriptSystem();
	m_debugger = new Debugger();
	m_collisionManager = new CollisionManager();

	m_renderer = new Renderer();

	InputHandler();

	Run();

	TerminateOpenGL();
}

void Application::Run()
{
	EngineUtils::Timer* Timer = EngineUtils::Timer::Instance();
	bool isRunning = true;
	// Locked to infinity for now, will change at later date
	constexpr float frameRate = std::numeric_limits<float>::infinity();

	onInit();

	while (!m_quit)
	{
		Timer->Tick();
		if (Timer->DeltaTime() >= 1 / frameRate)
		{
			Timer->Reset();
			glfwPollEvents();
			
			if (!m_pauseRuntime)
			{
				PhysicsSystem::Update(Timer->DeltaTime());
				InputHandler::PollGameControllers();
				m_collisionManager->update();
				onUpdate(Timer->DeltaTime());
				m_scriptSystem->OnUpdate(Timer->DeltaTime());
			}
			onRender(Timer->DeltaTime());
			m_scriptSystem->OnRender(Timer->DeltaTime());
			m_renderer->Render(Timer->DeltaTime());
		}
		
  }


	std::ofstream mFile("DebugLog.txt");

	for (int i = 0; i < m_debugger->GetDebug().size(); i++)
	{
		switch (m_debugger->GetDebug()[i].logLevel)
		{
		case LL_ERROR:
			mFile << m_debugger->GetDebug()[i].time << "[ERROR] " << m_debugger->GetDebug()[i].msg << std::endl;
			break;
		case LL_WARNING:
			mFile << m_debugger->GetDebug()[i].time << "[WARNING] " << m_debugger->GetDebug()[i].msg << std::endl;
			break;
		case LL_DEBUG:
			mFile << m_debugger->GetDebug()[i].time << "[DEBUG] " << m_debugger->GetDebug()[i].msg << std::endl;
			break;
		default:
			break;
		}
	}

	mFile.close();

	onQuit();
}

Application::~Application() 
{
	delete m_renderer;
	m_renderer = nullptr;
}

bool Application::ImportScene(const char* sceneName, bool setActive)
{
	if (!SceneImporter::ImportSceneFromFile(sceneName, setActive))
	{
		std::cerr << "Failed to import scene!" << std::endl;
		return false;
	}
	return true;
}

bool Application::ExportScene(const char* sceneName)
{
	if (!SceneExporter::ExportActiveSceneToFile("test scene.json"))
	{
		std::cerr << "Failed to export scene!" << std::endl;
		return false;
	}
	return true;
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

	return true;
}

void Application::Quit(InputEvent* e)
{
	m_quit = true;
}

void Application::Quit()
{
	m_quit = true;
}

bool Application::IsPaused()
{
	return m_pauseRuntime;
}

void Application::SetPause(bool pause)
{
	m_pauseRuntime = pause;
}

void Application::TogglePause()
{
	m_pauseRuntime = !m_pauseRuntime;
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

