#include "Physics.h"

void Physics::AddForce(glm::vec2 force)
{
	m_forces.push_back(force);
}

void Physics::Update(float dt)
{
	// Drag (laminar), 1.05f is our coefficient
	m_netForce += m_velocity * -1.99f;

	// Get net force
	for (glm::vec2 v : m_forces)
		m_netForce += v;
	m_forces.clear();

	// Derive acceleration with F = MA
	m_acceleration = m_netForce / m_mass;

	// Derive velocity by integrating acceleration
	m_velocity += m_acceleration * dt;

	// Zero out our force as we've cashed it in to make acceleration and velocity now
	m_netForce = glm::vec2(0, 0);
}
