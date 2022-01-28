#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

#include <iostream>
#include "source.h"

#include "EntityManager.h"
#include "ShaderFactory.h"

#include "Sprite.h"
#include "Timer.h"
#include "Transform.h"
#include "PlayerMovement.h"

#include "Camera.h"

extern Application* g_app;

void CheckGLErrors();

Renderer::Renderer() :
	m_defaultShader(ShaderFactory::CreatePipelineShader("Default", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")),
    m_postProcessingShader(ShaderFactory::CreatePipelineShader("Post-Processing", "PostProcessingV.glsl", "PostProcessingF.glsl")),
    m_sceneCamera(g_app->m_sceneManager->GetActiveScene()->m_entityManager->CreateEntity())
{
    m_projection = glm::mat4(1.0f);
    InitQuad();

    m_time = 0;

    m_gui = new ImGuiLayer();

    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();

    if (activeScene)
    {
        // Temporary until we're loading entities from file; need a camera for now
        Camera* cameraC = activeScene->m_entityManager->AddComponent<Camera>(m_sceneCamera, g_app->m_windowParams.windowWidth, g_app->m_windowParams.windowHeight, -1.0f, 1.0f, new Framebuffer);
        Transform* cameraTransform = activeScene->m_entityManager->AddComponent<Transform>(m_sceneCamera, glm::vec2(50.0f, 100.0f), glm::vec2(0.0f));

        m_projection = glm::orthoLH(0.0f, (float)g_app->m_windowParams.windowWidth, (float)g_app->m_windowParams.windowHeight, 0.0f, cameraC->m_near, cameraC->m_far);

        Entity e = activeScene->m_entityManager->CreateEntity();
        //activeScene->m_entityManager->AddComponent<Transform>(e, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        //activeScene->m_entityManager->AddComponent<Sprite>(e, TextureLoader::CreateTexture2DFromFile("testSpriteTexture", "hi.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);
    

        Entity e_testCharacter = activeScene->m_entityManager->CreateEntity();
        activeScene->m_entityManager->AddComponent<Transform>(e_testCharacter, glm::vec2(500.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        activeScene->m_entityManager->AddComponent <Sprite>(e_testCharacter, TextureLoader::CreateTexture2DFromFile("TestCharacter", "test.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);
        activeScene->m_entityManager->AddComponent<Physics>(e_testCharacter);
		activeScene->m_entityManager->AddComponent<PlayerMovement>(e_testCharacter);
    }
}

Renderer::~Renderer()
{
    delete m_defaultShader;
    m_defaultShader = nullptr;
}

void Renderer::DrawImGui()
{
    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    Camera* cameraComponent = activeScene->m_entityManager->GetComponent<Camera>(m_sceneCamera);

    m_gui->DrawMenuBar();
    m_gui->DrawHierachy();
    m_gui->DrawConsole();
    m_gui->DrawInspector();
    m_gui->DrawProfiler();
    m_gui->DrawSceneView();
}

void Renderer::DrawSprite(Sprite* sprite, Transform* transform)
{
    Shader* shader = sprite->GetShader();
    shader->Use();

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

void Renderer::Render(float deltaTime)
{
    m_gui->BeginGui();

    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();

    glViewport(0, 0, 1280, 720);

    if (activeScene)
    {
        Transform* cameraTransform = activeScene->m_entityManager->GetComponent<Transform>(m_sceneCamera);
        Camera* cameraComponent = activeScene->m_entityManager->GetComponent<Camera>(m_sceneCamera);
        glm::mat4 view = glm::mat4(1.0f);
        if (cameraTransform)
        {
            view = glm::translate(view, glm::vec3(cameraTransform->m_position, 0.0f));
            view = glm::rotate(view, cameraTransform->m_rotate, glm::vec3(0.0f, 0.0f, 1.0f));

            if (cameraComponent->m_framebuffer != nullptr)
            {
                cameraComponent->m_framebuffer->Bind();
                glEnable(GL_DEPTH_TEST);
            }
        }

        glClearColor(activeScene->m_sceneColour.r, activeScene->m_sceneColour.g, activeScene->m_sceneColour.b, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_defaultShader->Use();
        m_defaultShader->SetUniform("view", view);
        m_defaultShader->SetUniform("projection", m_projection);

        std::vector<Sprite*> sprites = activeScene->m_entityManager->GetAllComponentsOfType<Sprite>();
        for (Sprite* sprite : sprites)
        {
            Entity entity = activeScene->m_entityManager->GetEntityFromComponent<Sprite>(sprite);
            Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(entity);

            if (transform)
            {
                DrawSprite(sprite, transform);
            }
        }

        if (cameraComponent->m_framebuffer != nullptr)
        {
            m_gui->m_gameView->Bind();
            glDisable(GL_DEPTH_TEST);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_postProcessingShader->Use();
            m_postProcessingShader->SetUniform("effects", glm::vec3(1.0f));
            m_postProcessingShader->SetUniform("time", m_time);

            glBindTexture(GL_TEXTURE_2D, cameraComponent->m_framebuffer->GetRenderTextureID());
            glBindVertexArray(m_quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            cameraComponent->m_framebuffer->Unbind();

            DrawImGui();
        }
    }

	
    m_gui->EndGui();
	glfwSwapBuffers(g_app->m_window);

    m_time += deltaTime;
}

void Renderer::InitQuad()
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CheckGLErrors()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "GL error! Code: " << error << std::endl;
    }
}