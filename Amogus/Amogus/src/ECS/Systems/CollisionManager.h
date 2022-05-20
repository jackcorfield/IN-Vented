#pragma once
#include <glm/glm.hpp>
#include <Handlers/SpatialHash.h>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void update();

	bool checkCollision(Entity a, Entity b);
	std::vector<Entity> potentialCollisions(Entity e);
	
private:
	SpatialHash* m_spatialHash;
	//static bool ResolveCollision(glm::vec2 axis, float min_a, float min_b, float max_a, float max_b, glm::vec2& mtvAxis, float& mtvDistance);
	//static void CheckCollision();
};

