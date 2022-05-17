#pragma once

#include "Sprite.h"
#include <map>
#include <string>
#include <vector>

#include <nlohmann/include/nlohmann/json.hpp>

struct Animation
{
	std::vector<unsigned int> frames;
	float frameTime;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Animation, frames, frameTime)


class AnimatedSprite : public Sprite
{
public:

	AnimatedSprite(Texture2D spriteSheet, const glm::vec2& frameSize, const glm::vec3& colour, Shader* shader);
	
	bool createAnimation(const std::string& name, std::vector<unsigned int> frames, float frameTime);
	bool setAnimation(const std::string& name);
	
	void update(float dt);

	void setCurrentFrame(unsigned int frame) { m_currentFrame = frame % m_numFrames; }
	unsigned int getCurrentFrame() const { return m_currentFrame; }

	glm::vec2 getFrameSize() const { return m_frameSize; }

	unsigned int getNumFrames() const { return m_numFrames; }

	std::map<std::string, Animation>& getAnimations() { return m_animations; }
	
private:
	glm::vec2 m_frameSize;
	unsigned int m_numFrames;

	float m_time;
	unsigned int m_currentFrame;
	
	std::map<std::string, Animation> m_animations;
	Animation* m_currentAnimation;
};

/*
* 
* Reference
* 
* int imageWidth = 640; -> m_texture.m_width
* int imageHeight = 480; -> m_texture.m_height
* int tileWidth = 32; -> m_tileSize.x
* int tileHeight = 32; -> m_tileSize.y
*
* int tilesPerRow = imageWidth / tileWidth;
* int tilesPerColumn = imageHeight / tileHeight;
* 
* int numTiles = tilesPerRow * tilesPerColumn; -> m_numFrames
* 
* int index = 5;
* 
* int left = tileWidth * (index % tilesPerRow);
* int top = tileHeight * (index / tilesPerRow);
* int bottom = top + tileHeight;
* int right = top + tileWidth;
* 
*/