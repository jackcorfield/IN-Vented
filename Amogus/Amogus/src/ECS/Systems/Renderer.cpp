#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <Core/source.h>

#include <ECS/EntityManager.h>
#include <ResourceFactories/ShaderFactory.h>

#include <Resources/Framebuffer.h>
#include <ECS/Components/Sprite.h>
#include <ECS/Components/AnimatedSprite.h>
#include <Core/Timer.h>
#include <ECS/Components/Transform.h>
#include <ECS/Components/PlayerMovement.h>
#include <ECS/Components/Audio.h>
#include <ECS/Components/Camera.h>
#include <ECS/Components/BoxCollider.h>

extern Application* g_app;

void CheckGLErrors();
void UpdateShaderCameraData(Shader* shader, const glm::mat4& view, const glm::mat4& projection);

Renderer::Renderer() :
    m_defaultShader(ShaderFactory::CreatePipelineShader("Default", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")),
    m_postProcessingShader(ShaderFactory::CreatePipelineShader("Post-Processing", "PostProcessingV.glsl", "PostProcessingF.glsl"))
{
    m_projection = glm::mat4(1.0f);
    InitQuad();

    m_time = 0;

    m_renderContext = { 0.0f, true, NULL, false };
}

Renderer::~Renderer()
{
    delete m_defaultShader;
    m_defaultShader = nullptr;
}

void Renderer::DrawSprite(Sprite* sprite, Transform* transform)
{
    Shader* shader = sprite->GetShader();
    if (shader == NULL)
        shader = m_defaultShader;
    shader->Use();

    // Translate, then rotate, then scale
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform->m_position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * transform->m_size.x, 0.5f * transform->m_size.y, 0.0f)); // Moves rotation origin to center
    model = glm::rotate(model, glm::radians(transform->m_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform->m_size.x, -0.5f * transform->m_size.y, 0.0f)); // Moves origin back

    model = glm::scale(model, glm::vec3(transform->m_size * 200.0f, 1.0f));

    shader->SetUniform("model", model);
    shader->SetUniform("depth", transform->m_depth);
    shader->SetUniform("spriteColor", sprite->GetColour());

    glActiveTexture(GL_TEXTURE0);
    shader->SetUniform("image", 0);
    sprite->GetTexture().Bind();

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    sprite->GetTexture().Unbind();
    glBindVertexArray(0);
}

void Renderer::DrawAnimatedSprite(AnimatedSprite* sprite, Transform* transform)
{
    Shader* shader = sprite->GetShader();
    if (shader == NULL)
        shader = m_defaultShader;
    shader->Use();

    // Translate, then rotate, then scale
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform->m_position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * transform->m_size.x, 0.5f * transform->m_size.y, 0.0f)); // Moves rotation origin to center
    model = glm::rotate(model, glm::radians(transform->m_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform->m_size.x, -0.5f * transform->m_size.y, 0.0f)); // Moves origin back

    model = glm::scale(model, glm::vec3(transform->m_size * 200.0f, 1.0f));

    shader->SetUniform("model", model);
    shader->SetUniform("depth", transform->m_depth);
    shader->SetUniform("spriteColor", sprite->GetColour());

    glActiveTexture(GL_TEXTURE0);
    shader->SetUniform("image", 0);
    sprite->GetTexture().Bind();

    int imageWidth = sprite->GetTexture().m_width;
	int imageHeight = sprite->GetTexture().m_height;
    int frameWidth = sprite->getFrameSize().x;
    int frameHeight = sprite->getFrameSize().y;

	int frameCount = sprite->getNumFrames();

	int currentFrame = sprite->getCurrentFrame();

    int framesPerRow = imageWidth / frameWidth;
	int frameX = frameWidth * (currentFrame % framesPerRow);
    int frameY = frameHeight * (currentFrame / framesPerRow);
	
	float uvLeft = (float)frameX / (float)imageWidth;
	float uvRight = (float)(frameX + frameWidth) / (float)imageWidth;
	float uvTop = (float)(frameY + frameHeight) / (float)imageHeight;
	float uvBottom = (float)frameY / (float)imageHeight;

	
	
    SetQuadUVs({ uvLeft, uvTop }, { uvRight, uvTop }, { uvLeft, uvBottom }, { uvRight, uvBottom });
	
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    sprite->GetTexture().Unbind();
    glBindVertexArray(0);
}

void Renderer::DrawScene()
{
    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    if (activeScene && m_sceneCamera != 0)
    {
        Camera* activeCamera = activeScene->m_entityManager->GetComponent<Camera>(m_sceneCamera);
        Transform* cameraTransform = activeScene->m_entityManager->GetComponent<Transform>(m_sceneCamera);

        m_projection = glm::orthoLH(0.0f, (float)g_app->m_windowParams.windowWidth, (float)g_app->m_windowParams.windowHeight, 0.0f, activeCamera->m_near, activeCamera->m_far);

        glm::mat4 view = glm::mat4(1.0f);
        if (activeCamera && cameraTransform)
        {
            view = glm::translate(view, glm::vec3(cameraTransform->m_position, 0.0f));
            view = glm::rotate(view, cameraTransform->m_rotate, glm::vec3(0.0f, 0.0f, 1.0f));

            if (activeCamera->m_framebuffer != nullptr && m_renderContext.postProcessing)
            {
                activeCamera->m_framebuffer->Bind();
                glEnable(GL_DEPTH_TEST);
            }
        }

        glClearColor(activeScene->m_sceneColour.r, activeScene->m_sceneColour.g, activeScene->m_sceneColour.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!activeCamera)
        {
            return;
        }

        m_defaultShader->Use();
        m_defaultShader->SetUniform("view", view);
        m_defaultShader->SetUniform("projection", m_projection);

        std::vector<AnimatedSprite*> animatedSprites = activeScene->m_entityManager->GetAllComponentsOfType<AnimatedSprite>();
        for (AnimatedSprite* aSprite : animatedSprites)
        {
			if (!g_app->IsPaused())
                aSprite->update(m_renderContext.deltaTime);

            Shader* shader = aSprite->GetShader();
            if (shader == NULL)
                shader = m_defaultShader;
            UpdateShaderCameraData(shader, view, m_projection);

            Entity entity = activeScene->m_entityManager->GetEntityFromComponent<AnimatedSprite>(aSprite);
            Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(entity);

            if (transform)
            {
                DrawAnimatedSprite(aSprite, transform);
            }
        }

        SetQuadUVs({ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f });

        std::vector<Sprite*> sprites = activeScene->m_entityManager->GetAllComponentsOfType<Sprite>();
        for (Sprite* sprite : sprites)
        {
            Entity entity = activeScene->m_entityManager->GetEntityFromComponent<Sprite>(sprite);
            Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(entity);

            Shader* shader = sprite->GetShader();
            if (shader == NULL)
                shader = m_defaultShader;
            UpdateShaderCameraData(shader, view, m_projection);

            if (transform)
            {
                DrawSprite(sprite, transform);
            }
        }

        if (activeCamera->m_framebuffer != NULL && m_renderContext.postProcessing)
        {
            PostProcessScene();
        }
    }
}

void Renderer::PostProcessScene()
{
    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    Camera* activeCamera = activeScene->m_entityManager->GetComponent<Camera>(m_sceneCamera);

    activeCamera->m_framebuffer->Unbind();
    if (m_renderContext.framebuffer != NULL)
        m_renderContext.framebuffer->Bind();
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_postProcessingShader->Use();
    m_postProcessingShader->SetUniform("effects", glm::vec3(m_renderContext.screenShake, 0.0f, 0.0f));
    m_postProcessingShader->SetUniform("time", m_time);

    glBindTexture(GL_TEXTURE_2D, activeCamera->m_framebuffer->GetRenderTextureID());
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    activeCamera->m_framebuffer->Unbind();
}

void Renderer::Render(float deltaTime)
{
    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    m_renderContext.deltaTime = deltaTime;

    DrawScene();

    // ImGui call is separate, allowing DrawScene to be terminated early without stopping ImGui from rendering
    g_app->onImGui();

    glfwSwapBuffers(g_app->m_window);

    m_time += deltaTime;
}

void Renderer::SetActiveCamera(Entity cameraEntity)
{
    Scene* scene = g_app->m_sceneManager->GetActiveScene();
    EntityManager* entityManager = scene->m_entityManager;

    if (entityManager->IsEntity(m_sceneCamera))
    {
        if (entityManager->HasComponent<Camera>(m_sceneCamera))
        {
            Camera* oldCamera = entityManager->GetComponent<Camera>(m_sceneCamera);
            oldCamera->m_isActive = false;
        }
    }

    m_sceneCamera = cameraEntity;
    if (entityManager->IsEntity(m_sceneCamera))
    {
        Camera* newCamera = entityManager->GetComponent<Camera>(m_sceneCamera);
        if (newCamera)
        {
            newCamera->m_isActive = true;
        }
    }
}

void Renderer::setPostProcessing(bool postProcessing)
{
    m_renderContext.postProcessing = postProcessing;
}

void Renderer::setFramebuffer(Framebuffer* fb)
{
    m_renderContext.framebuffer = fb;
}

void Renderer::setScreenShake(bool screenShake)
{
    m_renderContext.screenShake = screenShake;
}

void Renderer::InitQuad()
{
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
    glGenBuffers(1, &m_quadVBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)offsetof(SpriteVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::SetQuadUVs(glm::vec2 tl, glm::vec2 tr, glm::vec2 bl, glm::vec2 br)
{
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

	SpriteVertex vertices[] = {
		// pos      // texCoords
        { {-0.5f, 0.5f}, tl }, // TL
        { {0.5f, -0.5f}, br }, // BR
        { {-0.5f, -0.5f}, bl }, // BL

        { {-0.5f, 0.5f}, tl }, // TL
        { {0.5f, 0.5f}, tr }, // TR
        { {0.5f, -0.5f}, br } // BR
	};
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void CheckGLErrors()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        g_app->m_debugger->Log("GL error! Code: " + std::to_string(error), LL_ERROR);
        //std::cerr << "GL error! Code: " << error << std::endl;
    }
}

void UpdateShaderCameraData(Shader* shader, const glm::mat4& view, const glm::mat4& projection)
{
    shader->Use();
    shader->SetUniform("view", view);
    shader->SetUniform("projection", projection);
    shader->Unuse();
}