#pragma once
#include <glm/glm.hpp>
#include <Handlers/SpatialHash.h>

class BoxCollider;
class Transform;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void update();

	bool checkCollision(Entity a, Entity b);
	std::vector<Entity> potentialCollisions(Entity e);

	bool BoxVBoxCollision(const BoxCollider* collider1, const Transform* transform1, const BoxCollider* collider2, const Transform* transform2);
	
private:
	SpatialHash* m_spatialHash;
	//static bool ResolveCollision(glm::vec2 axis, float min_a, float min_b, float max_a, float max_b, glm::vec2& mtvAxis, float& mtvDistance);
	//static void CheckCollision();
};

