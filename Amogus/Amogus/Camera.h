#pragma once

class Camera
{
public:
	Camera() :
		Camera(0, 0)
	{}
	Camera(const int viewportWidth, const int viewportHeight) :
		m_viewportWidth(viewportWidth),
		m_viewportHeight(viewportHeight)
	{}

	int m_viewportWidth;
	int m_viewportHeight;

	//glm::mat4 view;
};