#include "Physics.h"

Physics::Physics(Transform* transform)
	: m_transform(transform)
{
	SetPosition(m_transform->m_position);
	SetGravity(10.0f);
	SetMass(0.7f);

	m_gravityForce = glm::vec2(0.0f, -m_gravity) * m_mass;
	m_netForce, m_thrustForce, m_frictionForce, m_dragForce = glm::vec2(0.0f, 0.0f);
}

Physics::~Physics()
{

}



void Physics::ConstantVelocity(float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime);
}

void Physics::ConstantAcceleration(float deltaTime)
{
	m_velocity = m_velocity + m_acceleration * deltaTime;
}

void Physics::CalculateMovement(const float deltaTime)
{
	// update world position of object by adding displacement to previously calculated position
	m_position = addScaledVector(m_position, m_velocity, deltaTime);

	//ConstantAcceleration(deltaTime);
	//m_position = addScaledVector(m_position, m_acceleration, 0.5 * deltaTime * deltaTime);

	SetPosition(m_position);

	m_transform->m_position = m_position;
}