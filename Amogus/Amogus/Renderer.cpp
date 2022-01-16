#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "source.h"

#include "ShaderFactory.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

extern Application* g_app;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::DrawImGui()
{
	// AP - ImGui rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello world!");
	ImGui::Text("Hi hi hi hi hi hi");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cool rendering stuff

	DrawImGui();

	glfwSwapBuffers(g_app->m_window);
}