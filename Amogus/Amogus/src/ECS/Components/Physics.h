#pragma once
#include <glm/glm.hpp>
#include <vector>

class Physics
{
public:
	Physics() : m_mass(10.0f), m_netForce(0.0f, 0.0f), m_acceleration(0.0f, 0.0f), m_velocity(0.0f, 0.0f) {}
	Physics(float mass) : m_mass(mass), m_netForce(0.0f, 0.0f), m_acceleration(0.0f, 0.0f), m_velocity(0.0f, 0.0f) {}

	void Update(float dt);
	void AddForce(glm::vec2 force);

	float m_mass;
	
	std::vector<glm::vec2> m_forces;
	glm::vec2 m_netForce;
	glm::vec2 m_acceleration;
	glm::vec2 m_velocity;
};