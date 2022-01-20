#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

#include <iostream>
#include "source.h"

#include "EntityManager.h"
#include "ShaderFactory.h"
#include "Sprite.h"
#include "Transform.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Fonts/IconHeader.h"

#include "Camera.h"

extern Application* g_app;

void CheckGLErrors();

Renderer::Renderer() :
	m_defaultShader(ShaderFactory::CreatePipelineShader("Default", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl")),
    m_currentCamera(g_app->m_entityManager->CreateEntity())
{
    // Temporary until we're loading entities from file; need a camera for now
    Camera* cameraC = g_app->m_entityManager->AddComponent<Camera>(m_currentCamera, g_app->m_windowParams.windowWidth, g_app->m_windowParams.windowHeight);
    Transform* cameraTransform = g_app->m_entityManager->AddComponent<Transform>(m_currentCamera, glm::vec2(50.0f, 100.0f), glm::vec2(0.0f));

    m_projection = glm::orthoLH(0.0f, (float)g_app->m_windowParams.windowWidth, (float)g_app->m_windowParams.windowHeight, 0.0f, cameraC->m_near, cameraC->m_far);

    InitQuad();

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
    static bool p_open = true;

	// AP - ImGui rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("MainDockSpace", &p_open, window_flags);

    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    //Top Menu Bar
    /////////////////////////////////////////////////////////////////
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem(ICON_FA_FILE"  New"))
                {
                    //Do something
                }
                else if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN" Open"))
                {

                }
                else if (ImGui::MenuItem(ICON_FA_SD_CARD"  Save"))
                {

                }
                else if (ImGui::MenuItem(ICON_FA_SAVE"  Save As"))
                {

                }
                else if (ImGui::MenuItem(ICON_FA_FILE_EXPORT" Export"))
                {

                }
                ImGui::EndMenu();

            }

            if (ImGui::BeginMenu("Assets"))
            {
                if (ImGui::BeginMenu("Add Component"))
                {
                    if (ImGui::MenuItem("Sprite"))
                    {

                    }
                    else if (ImGui::MenuItem("Camera"))
                    {

                    }
                    else if (ImGui::MenuItem("Tile Map"))
                    {

                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem(ICON_FA_COG" Settings"))
            {


            }


            ImGui::EndMainMenuBar();
        }
    }
    /////////////////////////////////////////////////////////////////

    //Hierachy///////////////////////////////////////////////////////
    {
        ImGui::Begin("Hierachy");

        ImGui::InputText("Search", inputString, sizeof(inputString));
        if (ImGui::TreeNode("Scene1"))
        {
            if (ImGui::TreeNode("Sprite1"))
            {
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Sprite2"))
            {
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Camera1"))
            {
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("TileMap"))
            {
                if (ImGui::TreeNode("Tile1"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile2"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile3"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile4"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile5"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile6"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile7"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile8"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Tile9"))
                {
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        ImGui::End();
    }
    /////////////////////////////////////////////////////////////////
    
    //Profiler///////////////////////////////////////////////////////
    {
        ImGui::Begin("Profiler");
        static float arr[] = { 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f, 16.0f, 20.0f, 10.0f, 12.0f, 14.0f, 11.0f, 19.0f };
        ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr), 0, "", 0, 30, ImVec2(ImGui::GetWindowWidth() - 100, 30));
        ImGui::End();
    }
    /////////////////////////////////////////////////////////////////
    
    //Console////////////////////////////////////////////////////////
    {
        ImGui::Begin("Console");
        ImGui::End();
    }
    /////////////////////////////////////////////////////////////////

    //Inspector//////////////////////////////////////////////////////
    {
        ImGui::Begin("Inspector");
        ImGui::InputText("Component Name", inputString, 32);

        if (ImGui::CollapsingHeader("Transform"))
        {
            ImGui::DragFloat3("Position", dragFloat, 0.1f, 0.1f, 0.1f, "%.3f");
            ImGui::DragFloat3("Rotation", dragFloat, 0.1f, 0.1f, 0.1f, "%.3f");
            ImGui::DragFloat3("Scale", dragFloat, 0.1f, 0.1f, 0.1f, "%.3f");
        }
        if (ImGui::CollapsingHeader("Sprite Renderer"))
        {
            ImGui::Text("Add some fun sprite stuff here idk");
        }
        

        ImGui::End();
    }
    /////////////////////////////////////////////////////////////////

    ImGui::End();
	ImGui::Render();

    
    

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
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

void Renderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    Transform* cameraTransform = g_app->m_entityManager->GetComponent<Transform>(m_currentCamera);

    glm::mat4 view = glm::mat4(1.0f);
    if (cameraTransform)
    {
        view = glm::translate(view, glm::vec3(cameraTransform->m_position, 0.0f));
        view = glm::rotate(view, cameraTransform->m_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    m_defaultShader->SetUniform("view", view);

    m_defaultShader->SetUniform("projection", m_projection);

    std::vector<Sprite*> sprites = g_app->m_entityManager->GetAllComponentsOfType<Sprite>();
    for (Sprite* sprite : sprites)
    {
        Entity entity = g_app->m_entityManager->GetEntityFromComponent<Sprite>(sprite);
        Transform* transform = g_app->m_entityManager->GetComponent<Transform>(entity);
        if (transform)
        {
            DrawSprite(sprite, transform);
        }
    }

	DrawImGui();

	glfwSwapBuffers(g_app->m_window);
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