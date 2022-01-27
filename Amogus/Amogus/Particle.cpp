#include "Particle.h"



Particle::Particle()
{
	m_ParticlePool.resize(1000);
}

void Particle::Render()
{

}

void Particle::Update(float dt)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= dt;
		particle.m_pos += particle.m_velocity * dt;
		particle.Rotation += 0.01f * dt;
	}
}

void Particle::Emit(const ParticleInfo& particles)
{
	Particles& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.m_pos = particles.m_pos;
	//particle.Rotation = random
	
	//Velocity
	particle.m_velocity = particles.m_velocity;
	//particle.m_velocity.x += particle.m_velocity.x * (random - 0.5f)
	//particle.m_velocity.y += particle.m_velocity.y * (random - 0.5f)

	//Colour


	//Lifetime
	particle.LifeTime = particles.LifeTime;
	particle.LifeRemaining = particles.LifeTime;

	//Size
	particle.m_startSize = particles.m_startSize; //*particles.m_variation * (random - 0.5f)
	particle.m_endSize = particles.m_endSize;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
