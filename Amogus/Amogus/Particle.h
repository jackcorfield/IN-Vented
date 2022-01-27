#pragma once
#include <glm.hpp>
#include <vector>

#include "Shader.h"
#include "Texture2D.h"

struct ParticleInfo
{
	glm::vec2 m_pos;
	glm::vec2 m_velocity, m_variation;
	float m_startSize, m_endSize, m_sizeVariation;
	float LifeTime = 1.f;
};

class Particle
{
public:
	Particle(const Texture2D& texture, const glm::vec3& colour = glm::vec3(1.0f), Shader* shader = nullptr); // texture
	Particle(const glm::vec3& colour = glm::vec3(1.0f), Shader* shader = nullptr); // colour 

	void Update(float dt);
	void Render();

	void Emit(const ParticleInfo& particles);

private:
	struct Particles
	{
		glm::vec2 m_pos;
		glm::vec2 m_velocity;
		float m_startSize, m_endSize;
		float Rotation = 0.0f;
		float LifeTime = 1.f;
		float LifeRemaining = 0.f;

		bool Active = false;
	};
	std::vector<Particles> m_ParticlePool;
	int m_PoolIndex = 999;

};

