#pragma once
#include "Sprite.h"
#include "Transform.h"

struct GLFWwindow;

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

private:
	void DrawImGui();
	void DrawSprite(Sprite* sprite, Transform* transform);

	Shader* m_defaultShader;
	unsigned int m_quadVAO;
};