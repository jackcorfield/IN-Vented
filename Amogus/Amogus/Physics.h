#pragma once
#include <glm.hpp>
#include <vector>

class Physics
{
public:
	Physics(std::vector<glm::vec2>* transform); //position, rotation, scale
	~Physics();

	bool m_gravity = true;

	void		UpdateVelocity() { m_velocity = m_velocity + m_acceleration; }
	void		UpdateAcceleration() { m_acceleration = m_netForce / m_mass; }

	void		SetPosition(glm::vec2 position) { m_position = position; }

	void		SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void		SetVelocity(float x, float y, float z) { m_velocity.x = x, m_velocity.y = y; }

	void		SetAcceleration(glm::vec2 acceleration) { m_acceleration = acceleration; }
	void		SetAcceleration(float x, float y, float z) { m_acceleration.x = x, m_acceleration.y = y; }

	void		SetGravity(float newGrav) { m_gravity = newGrav; }
	void		SetMass(float newMass) { m_mass = newMass; }
	void		SetDamping(float newDamp) { m_damping = newDamp; }

	glm::vec2 addScaledVector(glm::vec2 vec1, glm::vec2 vec2,  float scale)
	{
		vec1.x += vec2.x * scale;
		vec1.y += vec2.y * scale;

		return vec1;
	}

private:

	void		ConstantVelocity(float deltaTime);
	void		ConstantAcceleration(float deltaTime);
	void		CalculateMovement(const float deltaTime);

	float		m_gravity;
	float		m_mass;
	float		m_damping;

	glm::vec2		m_netForce;
	glm::vec2		m_gravityForce;
	glm::vec2		m_thrustForce;
	glm::vec2		m_frictionForce;
	glm::vec2		m_dragForce;

	glm::vec2		m_position;
	glm::vec2		m_velocity;
	glm::vec2		m_acceleration;

	std::vector <glm::vec2>* m_transform;
};

