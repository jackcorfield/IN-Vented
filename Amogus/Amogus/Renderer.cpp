#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

#include <iostream>
#include "source.h"

#include "EntityManager.h"
#include "ShaderFactory.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

extern Application* g_app;

void CheckGLErrors();

Renderer::Renderer() :
	m_defaultShader(ShaderFactory::CreatePipelineShader("Default", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"))
{   
    unsigned int VBO;
    SpriteVertex vertices[] = {
        // pos      // texCoords
        { {-0.5f, 0.5f}, {0.0f, 1.0f} },
        { {0.5f, -0.5f}, {1.0f, 0.0f} },
        { {-0.5f, -0.5f}, {0.0f, 0.0f} },

        { {-0.5f, 0.5f}, {0.0f, 1.0f} },
        { {0.5f, 0.5f}, {1.0f, 1.0f} },
        { {0.5f, -0.5f}, {1.0f, 0.0f} }
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)offsetof(SpriteVertex, texCoords));

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    Entity e = g_app->m_entityManager->CreateEntity();
    g_app->m_entityManager->AddComponent<Transform>(e, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
    g_app->m_entityManager->AddComponent<Sprite>(e, TextureLoader::CreateTexture2DFromFile("testSpriteTexture", "hi.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);
}

Renderer::~Renderer()
{
    delete m_defaultShader;
    m_defaultShader = nullptr;
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

void Renderer::DrawSprite(Sprite* sprite, Transform* transform)
{
    Shader* shader = sprite->GetShader();
    shader->Use();

    // Temp
    glm::mat4 projection = glm::orthoLH(0.0f, (float)g_app->m_windowParams.windowWidth, (float)g_app->m_windowParams.windowHeight, 0.0f, -1.0f, 1.0f);
    //glm::mat4 projection = glm::mat4(1.0f);
    shader->SetUniform("projection", projection);

    // Translate, then rotate, then scale
	glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform->m_position, 0.0f));  

    model = glm::translate(model, glm::vec3(0.5f * transform->m_size.x, 0.5f * transform->m_size.y, 0.0f)); // Moves rotation origin to center
    model = glm::rotate(model, glm::radians(transform->m_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform->m_size.x, -0.5f * transform->m_size.y, 0.0f)); // Moves origin back

    model = glm::scale(model, glm::vec3(transform->m_size * 200.0f, 1.0f));
  
	shader->SetUniform("model", model);
	shader->SetUniform("spriteColor", sprite->GetColour());
  
    glActiveTexture(GL_TEXTURE0);
    shader->SetUniform("image", 0);
    sprite->GetTexture().Bind();

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    sprite->GetTexture().Unbind();
    glBindVertexArray(0);
}

void Renderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Sprite*> sprites = g_app->m_entityManager->GetAllComponentsOfType<Sprite>();
    for (Sprite* sprite : sprites)
    {
        Entity entity = g_app->m_entityManager->GetEntityFromComponent<Sprite>(sprite);
        Transform* transform = g_app->m_entityManager->GetComponent<Transform>(entity);

        DrawSprite(sprite, transform);
    }

	DrawImGui();

	glfwSwapBuffers(g_app->m_window);
}

void CheckGLErrors()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "GL error! Code: " << error << std::endl;
    }
}