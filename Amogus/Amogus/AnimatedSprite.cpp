#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const std::vector<Texture2D>& frames, float interval, const glm::vec3& colour, Shader* shader)
	: Sprite(frames[0], colour, shader), m_frameInterval(interval), m_frames(frames)
{
	m_numFrames = frames.size();
}

void AnimatedSprite::Update(float dt)
{
	m_timeElapsed += dt;
	if (m_timeElapsed >= m_frameInterval)
	{
		m_currentFrame++;
		m_currentFrame %= m_numFrames;
		m_texture = m_frames[m_currentFrame];
		m_timeElapsed = 0;
	}
}
