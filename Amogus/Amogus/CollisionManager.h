#pragma once
#include "glm.hpp"

class TileMap;

class CollisionManager
{
public:
	static bool ResolveCollision(glm::vec2 axis, float min_a, float min_b, float max_a, float max_b, glm::vec2& mtvAxis, float& mtvDistance);
	static void CheckCollision();
};

