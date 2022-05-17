#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <ECS/EntityManager.h>
#include <Resources/Framebuffer.h>
#include <Resources/Shader.h>
#include <Resources/Texture2D.h>

struct GLFWwindow;

class AnimatedSprite;
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

struct Font_Character
{
	Texture2D m_texture;
	glm::ivec2 m_size;
	glm::ivec2 m_bearing;
	unsigned int m_advance;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void LoadFont(std::string font);

	void Render(float deltaTime);

	void SetActiveCamera(Entity cameraEntity);

	void setPostProcessing(bool postProcessing);
	void setFramebuffer(Framebuffer* fb);
	void setScreenShake(bool screenShake);

	Entity GetSceneCamera() const { return m_sceneCamera; }

private:
	void DrawSprite(Sprite* sprite, Transform* transform);
	void DrawAnimatedSprite(AnimatedSprite* sprite, Transform* transform);

	void DrawScene();
	void PostProcessScene();

	void DrawUI();
	void DrawUI_Element(UI_BaseElement* element);

	void InitQuad();
	void SetQuadUVs(glm::vec2 tl, glm::vec2 tr, glm::vec2 bl, glm::vec2 br);

	Shader* m_defaultShader;
	Shader* m_uiShader;
	Shader* m_postProcessingShader;
	unsigned int m_quadVAO;
	unsigned int m_quadVBO;
	

	std::map<char, Font_Character> m_fontCharacters;

	Entity m_sceneCamera;
	glm::mat4 m_projection;

	float m_time;

	RenderContext m_renderContext;
};