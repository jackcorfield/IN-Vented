#include "Physics.h"

Physics::Physics(std::vector<glm::vec2>* transform)
	: m_transform(transform)
{
	SetPosition(m_transform->at(0));
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
	glm::vec2 position = m_transform->at(0) + (m_velocity * deltaTime);
	m_transform->at(0) = glm::vec2(position + (m_velocity * deltaTime));
}

void Physics::ConstantAcceleration(float deltaTime)
{
	m_velocity = m_velocity + m_acceleration * deltaTime;
}

void Physics::CalculateMovement(const float deltaTime)
{
	// update world position of object by adding displacement to previously calculated position
	m_position = addScaledVector(m_position, m_velocity, deltaTime);
	m_position = addScaledVector(m_position, m_acceleration, 0.5 * deltaTime * deltaTime);
	//ConstantAcceleration(deltaTime);
	// update velocity of object by adding change relative to previously calculated velocity

	SetPosition(m_position);
}