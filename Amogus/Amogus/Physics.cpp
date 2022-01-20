#include "Physics.h"

Physics::Physics()
{
	SetGravity(10.0f);
	SetMass(0.7f);

	m_gravityForce = glm::vec2(0.0f, -m_gravity) * m_mass;
	m_netForce, m_thrustForce, m_frictionForce, m_dragForce = glm::vec2(0.0f, 0.0f);
}

Physics::~Physics()
{

}

void Physics::CalculateMovement(Transform* transform, const float deltaTime)
{
	// update world position of object by adding displacement to previously calculated position
	m_position = transform->m_position;
	m_position = addScaledVector(m_position, m_velocity, deltaTime);

	//ConstantAcceleration(deltaTime);
	//m_position = addScaledVector(m_position, m_acceleration, 0.5 * deltaTime * deltaTime);

	SetPosition(m_position);

	transform->m_position = m_position;
}