#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "source.h"

extern Application* g_app;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cool rendering stuff

	glfwSwapBuffers(g_app->m_window);
}