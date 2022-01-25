#pragma once

#include <gtc/matrix_transform.hpp>

#include "EntityManager.h"
#include "Shader.h"

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

	void Render();

	void SetActiveCamera(Entity cameraEntity);

private:
	void DrawImGui();
	void DrawSprite(Sprite* sprite, Transform* transform);

	void InitQuad();

	Shader* m_defaultShader;
	unsigned int m_quadVAO;

	Entity m_currentCamera;
	glm::mat4 m_projection;
};