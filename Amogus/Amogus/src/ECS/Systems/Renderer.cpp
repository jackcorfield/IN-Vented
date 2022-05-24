#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

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
#include <ECS/Components/UI_Widget.h>

extern Application* g_app;

void CheckGLErrors();
void UpdateShaderCameraData(Shader* shader, const glm::mat4& view, const glm::mat4& projection);

Renderer::Renderer() :
    m_defaultShader(ShaderFactory::CreatePipelineShader("Default", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")),
    m_postProcessingShader(ShaderFactory::CreatePipelineShader("Post-Processing", "PostProcessingV.glsl", "PostProcessingF.glsl")),
    m_uiShader(ShaderFactory::CreatePipelineShader("UI", "UIV.glsl", "UIF.glsl")),
	m_textShader(ShaderFactory::CreatePipelineShader("Text", "TextV.glsl", "TextF.glsl"))
{
    m_projection = glm::mat4(1.0f);
    InitQuad();

    m_time = 0;

    m_renderContext = { 0.0f, true, NULL, false };

    LoadFont("Fonts/Roboto-Regular.ttf");
}

Renderer::~Renderer()
{
    delete m_defaultShader;
    m_defaultShader = nullptr;

    delete m_uiShader;
    m_uiShader = nullptr;

    delete m_postProcessingShader;
    m_postProcessingShader = nullptr;
}

void Renderer::LoadFont(std::string font)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR INITIALIZING FREETYPE" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        std::cout << "ERROR LOADING FONT" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    // initialize font characters
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Error loading glyph" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Font_Character character = {
            Texture2D(texture, face->glyph->bitmap.width, face->glyph->bitmap.rows, std::string(1, c).c_str(), ""),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (face->glyph->advance.x >> 6)
        };
        m_fontCharacters.insert(std::pair<char, Font_Character>(c, character));
		//std::cout << "Character " << c << " loaded. Size: " << character.m_size.x << ", " << character.m_size.y << ". Bearing: " << character.m_bearing.x << ", " << character.m_bearing.y << ". Advance: " << character.m_advance << std::endl;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
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

	int currentFrameIndex = sprite->getCurrentFrame();

    int currentFrame = 0;
    Animation* currentAnimation = sprite->getCurrentAnimation();
    if (currentAnimation)
    {
        currentFrame = sprite->getCurrentAnimation()->frames[currentFrameIndex];
    }
    

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


        m_projection = glm::orthoLH(0.0f, activeCamera->m_internalWidth, activeCamera->m_internalHeight, 0.0f, activeCamera->m_near, activeCamera->m_far);

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

    DrawUI();

    m_renderContext.framebuffer->Unbind();
}

void Renderer::DrawUI()
{
    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    if (activeScene)
    {
        std::vector<UI_WidgetComponent*> widgets = activeScene->m_entityManager->GetAllComponentsOfType<UI_WidgetComponent>();

        for (UI_WidgetComponent* widget : widgets)
        {
			for (UI_BaseElement* element : widget->m_elements)
			{
				DrawUI_Element(element);
			}
        }
    }
}

void Renderer::DrawUI_Element(UI_BaseElement* element)
{
	if (element->m_hidden)
		return;

	switch (element->m_elementType)
	{
	case(ElementType::ET_Image):
	case(ElementType::ET_ImageButton):
	{
		UI_Image* imageElement = (UI_Image*)element;
		// View matrix is flat default
		// Projection matrix is default left handed ortho
		// Model is just position and size
		glm::mat4 view = glm::mat4(1.0f);

		m_uiShader->Use();
		m_uiShader->SetUniform("view", view);
		m_uiShader->SetUniform("projection", m_projection);
		m_uiShader->SetUniform("tint", imageElement->m_colour);
		
		float windowWidth = 0;
		float windowHeight = 0;
		if (g_app->m_windowParams.windowWidth != g_app->GetGameScreenInfo().z)
		{
			windowWidth = g_app->GetGameScreenInfo().z;
			windowHeight = g_app->GetGameScreenInfo().w;
		}
		else
		{
			windowWidth = g_app->m_windowParams.windowWidth;
			windowHeight = g_app->m_windowParams.windowHeight;
		}

		glm::vec2 adjustedRelativePos = element->m_relativePosition * glm::vec2(windowWidth, windowHeight);
		glm::vec3 finalPos = glm::vec3(adjustedRelativePos + element->m_absolutePosition, element->m_zIndex);

		// scaled from 0.0 to 1.0, gives us our pixel position relative to screen size
		glm::vec2 adjustedRelativeSize = element->m_relativeSize * glm::vec2(windowWidth, windowHeight);
		glm::vec3 finalSize = glm::vec3(adjustedRelativeSize + element->m_absoluteSize, 1);

		glm::mat4 model = glm::mat4(1.0f);

		// translate by position
		model = glm::translate(model, finalPos);
		model = glm::scale(model, finalSize);

		m_uiShader->SetUniform("model", model);

		glActiveTexture(GL_TEXTURE0);
		m_uiShader->SetUniform("image", 0);
		Texture2D* selectedTexture = nullptr;

		if (imageElement->m_elementType == ET_ImageButton)
		{
			UI_ImageButton* button = (UI_ImageButton*)imageElement;
			switch (button->m_state)
			{
			case(BS_None):
				selectedTexture = &button->m_texture;
				break;
			case(BS_Hover):
				selectedTexture = &button->m_hoveredTexture;
				break;
			case(BS_Click):
				selectedTexture = &button->m_clickedTexture;
				break;
			}
		}
		else
		{
			selectedTexture = &imageElement->m_texture;
		}

		selectedTexture->Bind();

		glBindVertexArray(m_quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		selectedTexture->Unbind();
		glBindVertexArray(0);
	}
	break;
	case(ElementType::ET_Text):
	{
        UI_Text* textElement = (UI_Text*)element;
        // View matrix is flat default
        // Projection matrix is default left handed ortho
        // Model is just position and size
        glm::mat4 view = glm::mat4(1.0f);

        m_textShader->Use();
		m_textShader->SetUniform("view", view);
		m_textShader->SetUniform("projection", m_projection);

		float windowWidth = 0;
		float windowHeight = 0;
		if (g_app->m_windowParams.windowWidth != g_app->GetGameScreenInfo().z)
		{
			windowWidth = g_app->GetGameScreenInfo().z;
			windowHeight = g_app->GetGameScreenInfo().w;
		}
		else
		{
			windowWidth = g_app->m_windowParams.windowWidth;
			windowHeight = g_app->m_windowParams.windowHeight;
		}

        glm::vec2 adjustedRelativePos = element->m_relativePosition * glm::vec2(windowWidth, windowHeight);
        glm::vec3 finalPos = glm::vec3(adjustedRelativePos + element->m_absolutePosition, element->m_zIndex);

        // scaled from 0.0 to 1.0, gives us our pixel position relative to screen size
        glm::vec2 adjustedRelativeSize = element->m_relativeSize * glm::vec2(windowWidth, windowHeight);
        glm::vec3 finalSize = glm::vec3(adjustedRelativeSize + element->m_absoluteSize, 1);
        float scale = finalSize.y;

		if (textElement->m_centered)
		{
			int sum = 0;
			for (int i = 0; i < textElement->m_text.size(); i++)
			{
				Font_Character ch = m_fontCharacters[textElement->m_text[i]];
				sum += (ch.m_advance * scale);
			}
			finalPos.x -= (sum / 2);
		}

        float x_offset = 0;

        for (int i = 0; i < textElement->m_text.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            Font_Character ch = m_fontCharacters[textElement->m_text[i]];

            float w = ch.m_size.x * scale;
            float h = ch.m_size.y * scale;
			float xpos = (x_offset + ch.m_bearing.x + w/2) * scale;
			float y_offset = (ch.m_size.y - ch.m_bearing.y) * scale;

            // translate by position
            model = glm::translate(model, (finalPos + glm::vec3(xpos, y_offset, 0)) - glm::vec3(0, h/2, 0));
            model = glm::scale(model, glm::vec3(w, h, 1));

			m_textShader->SetUniform("model", model);

            glActiveTexture(GL_TEXTURE0);
			m_textShader->SetUniform("image", 0);
			m_textShader->SetUniform("textColor", textElement->m_colour);
            ch.m_texture.Bind();

            glBindVertexArray(m_quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            ch.m_texture.Unbind();
            glBindVertexArray(0);

			x_offset += ch.m_advance * scale;
        }
	}
	break;
	default:
	case(ElementType::ET_Base):
	{

	}
	break;
	}
}

void Renderer::Render(float deltaTime)
{
    Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
    m_renderContext.deltaTime = deltaTime;

    DrawScene();
    //DrawUI();

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