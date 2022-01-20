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
