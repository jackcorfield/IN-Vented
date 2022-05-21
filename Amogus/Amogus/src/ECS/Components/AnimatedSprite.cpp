#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(Texture2D spriteSheet, const glm::vec2& frameSize, const glm::vec3& colour, Shader* shader) :
	Sprite(spriteSheet, colour, shader),
	m_frameSize(frameSize),
	m_time(0.0f),
	m_currentFrame(0),
	m_currentAnimation(NULL)
{
	int tilesPerRow = m_texture.m_width / m_frameSize.x;
	int tilesPerCol = m_texture.m_height / m_frameSize.y;
	m_numFrames = tilesPerRow * tilesPerCol;
}

bool AnimatedSprite::createAnimation(const std::string& name, std::vector<unsigned int> frames, float frameTime)
{
	if (m_animations.count(name) == 1)
		return false;

	for (unsigned int frame : frames)
	{
		if (frame >= m_numFrames)
		{
			return false;
		}
	}
	m_animations[name] = { frames, frameTime };
	return true;
}

bool AnimatedSprite::setAnimation(const std::string& name)
{
	if (m_animations.count(name) == 0)
		return false;
	m_currentAnimation = &m_animations[name];
	m_currentFrame = 0;

	return true;
}

void AnimatedSprite::update(float dt)
{
	if (m_currentAnimation)
	{
		m_time += dt;
		if (m_time >= m_currentAnimation->frameTime)
		{
			m_currentFrame++;
			m_currentFrame %= m_currentAnimation->frames.size();
			m_time = 0;
		}
	}
}