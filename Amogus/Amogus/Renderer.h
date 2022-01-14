#pragma once

struct GLFWwindow;

struct WindowParams
{
	unsigned int windowWidth, windowHeight;
	bool isFullscreen;
	unsigned int MSAASamples;
	const char* windowTitle;
};

class Renderer
{
public:
	Renderer(const WindowParams& windowParams);
	~Renderer();

	void Render();

private:
	bool InitOpenGL();
	void TerminateOpenGL();

	GLFWwindow* m_window;
	WindowParams m_windowParams;
};