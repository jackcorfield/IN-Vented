#pragma once

#include <glm/gtx/hash.hpp> // for std::hash
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

#include <ECS/EntityManager.h>

class SpatialHash
{
public:
	SpatialHash(unsigned int m_cellSize);
	glm::vec2 hash(const glm::vec2& position);

	bool insert(Entity entity);
	bool remove(Entity entity);

	void forceRemove(Entity entity);

	void updateCell(const glm::vec2& cellid);
	void updateAll();
	
	std::vector<Entity> broadCollisionCheck(Entity entity);

	bool isInHash(Entity e) const;
private:
	unsigned int m_cellSize;

	struct Cell
	{
		std::vector<Entity> contents;
	};

	std::unordered_map<glm::vec2, Cell> m_map;

	struct Entry
	{
		Entity entityID;
		glm::vec2 entryPosition;
	};

	std::vector<Entry> m_allEntities;
};