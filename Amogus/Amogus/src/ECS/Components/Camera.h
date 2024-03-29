#pragma once

#include <Resources/Framebuffer.h>

class Camera
{
public:
	Camera() :
		Camera(0.0f, 0.0f, -1.0f, 1.0f, nullptr)
	{}
	Camera(const float viewportWidth, const float viewportHeight, const float near = -1.0f, const float far = 1.0f, Framebuffer* renderTarget = nullptr) :
		m_viewportWidth(viewportWidth),
		m_viewportHeight(viewportHeight),
		m_internalWidth(viewportWidth),
		m_internalHeight(viewportHeight),
		m_near(near),
		m_far(far),
		m_framebuffer(renderTarget),
		m_isActive(false)
	{}

	float m_viewportWidth, m_viewportHeight;
	float m_internalWidth, m_internalHeight;
	float m_near, m_far;
	bool m_isActive; // Used for reading/writing active camera to JSON. If you want to change the active camera, do so through g_app->SetActiveCamera()
	Framebuffer* m_framebuffer;

	//glm::mat4 view;
};