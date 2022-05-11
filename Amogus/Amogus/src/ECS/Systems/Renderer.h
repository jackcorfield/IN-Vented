#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <ECS/EntityManager.h>
#include <Resources/Framebuffer.h>
#include <Resources/Shader.h>

struct GLFWwindow;

class Scene;
class Sprite;
class Transform;
class UI_BaseElement;

struct SpriteVertex
{
	glm::vec2 pos;
	glm::vec2 texCoords;
};

struct RenderContext
{
	float deltaTime;
	bool postProcessing;
	Framebuffer* framebuffer;
	bool screenShake;
};


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render(float deltaTime);

	void SetActiveCamera(Entity cameraEntity);

	void setPostProcessing(bool postProcessing);
	void setFramebuffer(Framebuffer* fb);
	void setScreenShake(bool screenShake);

	Entity GetSceneCamera() const { return m_sceneCamera; }

private:
	void DrawSprite(Sprite* sprite, Transform* transform);

	void DrawScene();
	void PostProcessScene();

	void DrawUI();
	void DrawUI_Element(UI_BaseElement* element);

	void InitQuad();

	Shader* m_defaultShader;
	Shader* m_uiShader;
	Shader* m_postProcessingShader;
	unsigned int m_quadVAO;

	Entity m_sceneCamera;
	glm::mat4 m_projection;

	float m_time;

	RenderContext m_renderContext;
};