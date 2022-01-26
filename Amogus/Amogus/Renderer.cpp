#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

#include <iostream>
#include "source.h"

#include "EntityManager.h"
#include "ShaderFactory.h"

#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Timer.h"
#include "Transform.h"
#include "PlayerMovement.h"
#include "Audio.h"
#include "Camera.h"

#include "BoxCollider.h"

extern Application* g_app;

void CheckGLErrors();

Renderer::Renderer() :
	m_defaultShader(ShaderFactory::CreatePipelineShader("Default", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")),
    m_postProcessingShader(ShaderFactory::CreatePipelineShader("Post-Processing", "PostProcessingV.glsl", "PostProcessingF.glsl")),
    m_currentCamera(g_app->m_sceneManager->GetActiveScene()->m_entityManager->CreateEntity())
{
    m_projection = glm::mat4(1.0f);
    InitQuad();

    m_time = 0;

    m_gui = new ImGuiLayer();

    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();

    if (activeScene)
    {
        // Temporary until we're loading entities from file; need a camera for now
        Camera* cameraC = activeScene->m_entityManager->AddComponent<Camera>(m_currentCamera, g_app->m_windowParams.windowWidth, g_app->m_windowParams.windowHeight, -1.0f, 1.0f, new Framebuffer);
        Transform* cameraTransform = activeScene->m_entityManager->AddComponent<Transform>(m_currentCamera, glm::vec2(50.0f, 100.0f), glm::vec2(0.0f));

        m_projection = glm::orthoLH(0.0f, (float)g_app->m_windowParams.windowWidth, (float)g_app->m_windowParams.windowHeight, 0.0f, cameraC->m_near, cameraC->m_far);

        Entity e = activeScene->m_entityManager->CreateEntity();
        //activeScene->m_entityManager->AddComponent<Transform>(e, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        //activeScene->m_entityManager->AddComponent<Sprite>(e, TextureLoader::CreateTexture2DFromFile("testSpriteTexture", "hi.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);
  
        Entity e_testCharacter = activeScene->m_entityManager->CreateEntity();
        Transform* testTransform = activeScene->m_entityManager->AddComponent<Transform>(e_testCharacter, glm::vec2(500.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        Sprite* testSprite = activeScene->m_entityManager->AddComponent<Sprite>(e_testCharacter, TextureLoader::CreateTexture2DFromFile("TestCharacter", "test.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);

        activeScene->m_entityManager->AddComponent<Transform>(e_testCharacter, glm::vec2(500.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
		activeScene->m_entityManager->AddComponent<AnimatedSprite>(e_testCharacter,
			std::vector<Texture2D>{TextureLoader::CreateTexture2DFromFile("TestCharacter", "test.png"), TextureLoader::CreateTexture2DFromFile("TestCharacter", "test2.png")},
			0.5f,
			glm::vec3(1.0f, 1.0f, 1.0f), 
			m_defaultShader);
        activeScene->m_entityManager->AddComponent<Physics>(e_testCharacter);
      	activeScene->m_entityManager->AddComponent<PlayerMovement>(e_testCharacter);
        activeScene->m_entityManager->AddComponent<BoxCollider>(e_testCharacter, testTransform->m_position, glm::vec2(testTransform->m_size.x * 100.0f, testTransform->m_size.y * 100.0f));
      
        //this is for memes pls delete
        Entity e_420truck = activeScene->m_entityManager->CreateEntity();
        Transform* e_420truckTransform = activeScene->m_entityManager->AddComponent<Transform>(e_420truck, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        Sprite* e_420truckSprite = activeScene->m_entityManager->AddComponent <Sprite>(e_420truck, TextureLoader::CreateTexture2DFromFile("420truck", "Assets/Sprites/420truck.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);
        activeScene->m_entityManager->AddComponent<Audio>(e_420truck, "Assets/Audio/Diesel.wav", g_app->m_audioManager->m_system, g_app->m_audioManager->bgm);
		activeScene->m_entityManager->AddComponent<BoxCollider>(e_420truck, e_420truckTransform->m_position, glm::vec2(e_420truckTransform->m_size.x * 350.0f, e_420truckTransform->m_size.y * 350.0f));

        Entity e_69truck = activeScene->m_entityManager->CreateEntity();
        Transform* e_69truckTransform = activeScene->m_entityManager->AddComponent<Transform>(e_69truck, glm::vec2(500.0f, 400.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        Sprite* e_69truckSprite = activeScene->m_entityManager->AddComponent <Sprite>(e_69truck, TextureLoader::CreateTexture2DFromFile("420truck", "Assets/Sprites/69truck.png"), glm::vec3(1.0f, 1.0f, 1.0f), m_defaultShader);
        activeScene->m_entityManager->AddComponent<Audio>(e_69truck, "Assets/Audio/grenade.wav", g_app->m_audioManager->m_system, g_app->m_audioManager->sfx);
       
        //audio manager testing
        g_app->m_audioManager->SetVolume(g_app->m_audioManager->bgm, 0.1f);
        g_app->m_audioManager->SetVolume(g_app->m_audioManager->sfx, 0.02f);

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
    Camera* cameraComponent = activeScene->m_entityManager->GetComponent<Camera>(m_currentCamera);

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

    glViewport(0, 0, m_gui->GetFrameSize().x, m_gui->GetFrameSize().y);

    if (activeScene)
    {
        Transform* cameraTransform = activeScene->m_entityManager->GetComponent<Transform>(m_currentCamera);
        Camera* cameraComponent = activeScene->m_entityManager->GetComponent<Camera>(m_currentCamera);

        if (m_gui->m_sceneFrameResized)
        {
            cameraComponent->m_viewportWidth = m_gui->GetFrameSize().x;
            cameraComponent->m_viewportHeight = m_gui->GetFrameSize().y;
            cameraComponent->m_framebuffer->Resize(m_gui->GetFrameSize().x, m_gui->GetFrameSize().y);
            m_projection = glm::orthoLH(0.0f, m_gui->GetFrameSize().x, m_gui->GetFrameSize().y, 0.0f, cameraComponent->m_near, cameraComponent->m_far);

            m_gui->m_sceneFrameResized = false;
        }
        
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

		std::vector<AnimatedSprite*> animatedSprites = activeScene->m_entityManager->GetAllComponentsOfType<AnimatedSprite>();
		for (AnimatedSprite* aSprite : animatedSprites)
		{
			aSprite->Update(deltaTime);

			Entity entity = activeScene->m_entityManager->GetEntityFromComponent<AnimatedSprite>(aSprite);
			Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(entity);

			if (transform)
			{
				DrawSprite(aSprite, transform);
			}
		}

		std::vector<Sprite*> sprites = activeScene->m_entityManager->GetAllComponentsOfType<Sprite>();
        for (Sprite* sprite : sprites)
        {
            Entity entity = activeScene->m_entityManager->GetEntityFromComponent<Sprite>(sprite);
            Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(entity);

            if (transform)
            {
                DrawSprite(sprite, transform);
            }

            //this is for testing pls dont get angry at me : (
            Audio*  audio = activeScene->m_entityManager->GetComponent<Audio>(entity);

            if (audio && !audio->IsPlaying)
            {
                audio->PlayAudio();
            }
        }

        if (cameraComponent->m_framebuffer != nullptr)
        {
            cameraComponent->m_framebuffer->Unbind();
            glDisable(GL_DEPTH_TEST);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_postProcessingShader->Use();
            m_postProcessingShader->SetUniform("effects", glm::vec3(0.0f, 0.0f, 0.0f)); // r = screen shake, g = hdr, b = 
            m_postProcessingShader->SetUniform("time", m_time);

            glBindTexture(GL_TEXTURE_2D, 1);
            DrawImGui();
        }
    }

	
    m_gui->EndGui();
	glfwSwapBuffers(g_app->m_window);

    m_time += deltaTime;
    // screen shake relies on an incrementing time rather than deltatime
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