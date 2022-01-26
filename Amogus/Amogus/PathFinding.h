#pragma once
#include <list>
#include <string>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include "Transform.h"

class Tile;
class TileMap;
class Entity;

struct PathfindingNode
{
public:
	PathfindingNode* m_parent;
	glm::vec2 m_point;
	double m_fCost;
	bool m_closed;
	bool m_open;

	PathfindingNode(glm::vec2 point)
		: m_point(point),
		m_fCost(0.0f),
		m_closed(false),
		m_open(false),
		m_parent(nullptr)
	{

	}

	void DeriveHeuristics(PathfindingNode* start, PathfindingNode* end);
};

class PathfindingHandler
{
public:
	static std::vector<Entity> CalculatePath(TileMap* tilemap, const glm::vec2 start, const glm::vec2 end);
};
