#pragma once
#include <glm.hpp>
#include <vector>

class Physics
{
public:
	Physics() : m_mass(10.0f), m_netForce(0, 0), m_acceleration(0, 0), m_velocity(0, 0) {}

	void Update(float dt);
	void AddForce(glm::vec2 force);

	float m_mass;
	
	std::vector<glm::vec2> m_forces;
	glm::vec2 m_netForce;
	glm::vec2 m_acceleration;
	glm::vec2 m_velocity;
};