#pragma once
#include "glm.hpp"

class CircleCollider
{
public:
	CircleCollider(float radius, glm::vec2 centre);
	void SetSize(float radius) { m_radius = radius; };//Function will just update width and height variable of collider
	void CheckCollision();

	bool CircleOverlap(float targetRadius, glm::vec2 targetCentre) { return fabs((m_centre.x - targetCentre.x) * (m_centre.x - targetCentre.x) + (m_centre.y - targetCentre.y) * (m_centre.y - targetCentre.y)) <= (m_radius + targetRadius)*(m_radius+targetRadius); }
	float	DistanceCircleToCircle(glm::vec2 targetCentre) { float distance = sqrtf((m_centre.x - targetCentre.x) * (m_centre.x - targetCentre.x) + (m_centre.y - targetCentre.y) * (m_centre.y - targetCentre.y)); };

	glm::vec2 m_centre;
	float m_radius;
};

