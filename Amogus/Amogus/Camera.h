#pragma once

class Camera
{
public:
	Camera() :
		Camera(0.0f, 0.0f, -1.0f, 1.0f)
	{}
	Camera(const float viewportWidth, const float viewportHeight, const float near = -1.0f, const float far = 1.0f) :
		m_viewportWidth(viewportWidth),
		m_viewportHeight(viewportHeight),
		m_near(near),
		m_far(far)
	{}

	float m_viewportWidth, m_viewportHeight;
	float m_near, m_far;
	bool m_isActive; // Used for reading/writing active camera to JSON. If you want to change the active camera, do so through g_app->SetActiveCamera()

	//glm::mat4 view;
};