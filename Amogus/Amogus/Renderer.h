#pragma once
#include "Sprite.h"

struct GLFWwindow;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();

private:
	void DrawImGui();
	void DrawSprite(Sprite* sprite);

	Shader* m_defaultShader;
	Sprite* m_sprite;
	unsigned int m_quadVAO;
};