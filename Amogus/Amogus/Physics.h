#pragma once
#include <glm.hpp>
#include <vector>

class Physics
{
public:
	Physics();
	~Physics();

	void		UpdateVelocity() { m_velocity = m_velocity + m_acceleration; }
	void		UpdateAcceleration() { m_acceleration = m_netForce / m_mass; }
	void		UpdateNetForce() 
	{ 
		m_netForce = m_thrustForce + m_frictionForce; 
		m_thrustForce = glm::vec2(0, 0);
		m_frictionForce = glm::vec2(0, 0);
	}
	
	void		ResetNetForce() { m_netForce = glm::vec2(0, 0); m_velocity = glm::vec2(0, 0);}

	void				SetPosition(glm::vec2 position) { m_position = position; }
	glm::vec2		GetPosition() { return m_position; }

	void			SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void			SetVelocity(float x, float y) { m_velocity.x = x, m_velocity.y = y; }
	glm::vec2	GetVelocity() { return m_velocity; }

	void		SetAcceleration(glm::vec2 acceleration) { m_acceleration = acceleration; }
	void		SetAcceleration(float x, float y) { m_acceleration.x = x, m_acceleration.y = y; }

	void		SetGravity(float newGrav) { m_gravity = newGrav; }
	void		SetMass(float newMass) { m_mass = newMass; }
	void		SetDamping(float newDamp) { m_damping = newDamp; }

	void		SetThrust(glm::vec2 thrust) { m_thrustForce = thrust; }
	void		SetThrust(float x, float y) { m_thrustForce = glm::vec2(x,y); }

	glm::vec2 addScaledVector(glm::vec2 vec1, glm::vec2 vec2,  float scale)
	{
		vec1.x += vec2.x * scale;
		vec1.y += vec2.y * scale;

		return vec1;
	}

private:

	float		m_gravity;
	float		m_mass;
	float		m_damping;

	glm::vec2		m_netForce;
	glm::vec2		m_gravityForce;
	glm::vec2		m_thrustForce;
	glm::vec2		m_frictionForce = glm::vec2(0,0);
	glm::vec2		m_dragForce;

	glm::vec2		m_position;
	glm::vec2		m_velocity;
	glm::vec2		m_acceleration;

};

