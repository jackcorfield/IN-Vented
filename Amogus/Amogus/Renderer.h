#pragma once

#include <gtc/matrix_transform.hpp>

#include "EntityManager.h"
#include "Shader.h"

#include "ImGuiLayer.h"

struct GLFWwindow;

class Sprite;
class Transform;

struct SpriteVertex
{
	glm::vec2 pos;
	glm::vec2 texCoords;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render(float deltaTime);

private:
	void DrawImGui();
	void DrawSprite(Sprite* sprite, Transform* transform);

	void InitQuad();

	Shader* m_defaultShader;
	Shader* m_postProcessingShader;
	unsigned int m_quadVAO;

	Entity m_currentCamera;
	glm::mat4 m_projection;

	ImGuiLayer* m_gui;
};