#pragma once

#include "Sprite.h"
#include <vector>

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(const std::vector<Texture2D>& frames, float interval, const glm::vec3& colour = glm::vec3(1.0f), Shader* shader = nullptr);
	void Update(float dt);
private:
	std::vector<Texture2D> m_frames;

	int m_currentFrame = 0;
	int m_numFrames = -1;

	float m_timeElapsed = 0.0f;
	float m_frameInterval;
};