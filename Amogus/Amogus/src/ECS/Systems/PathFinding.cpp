#include "PathFinding.h"
#include <iostream>
#include <queue>
#include <ECS/Components/TileMap.h>
#include <ECS/Components/Tile.h>

#include <ECS/EntityManager.h>
#include <Core/source.h>

extern Application* g_app;

void PathfindingNode::DeriveHeuristics(PathfindingNode* start, PathfindingNode* end)
{
	double gCost = 0;
	PathfindingNode* cursor = this;

	// G Cost: exact vertices from the start
	while (cursor->m_parent != nullptr)
	{
		cursor = cursor->m_parent;
		gCost++;
	}

	// H Cost: euclidean distance from the end
	double hCost = sqrt(pow((end->m_point.x, m_point.x), 2.0f) + pow((end->m_point.y, m_point.y), 2.0f));

	m_fCost = hCost + gCost;
}

std::vector<Entity> PathfindingHandler::CalculatePath(TileMap* tilemap, const glm::vec2 start, const glm::vec2 end)
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();

	// Our cache for the path we're making
	std::map<std::pair<float, float>, PathfindingNode*> cache = std::map<std::pair<float, float>, PathfindingNode*>();
	auto GetNode = [&](const glm::vec2 pos)
	{
		if (cache.count(std::pair<float, float>(pos.x, pos.y)) == 0)
			cache[std::pair<float, float>(pos.x, pos.y)] = new PathfindingNode(pos);

		return cache[std::pair<float, float>(pos.x, pos.y)];
	};

	// Start and end point
	PathfindingNode* startNode = GetNode(tilemap->ConvertPositionToTileIndex(start));
	PathfindingNode* endNode = GetNode(tilemap->ConvertPositionToTileIndex(end));

	// Priority queue so we always get the one with the lowest fCost when we pop
	auto cmp = [](PathfindingNode* left, PathfindingNode* right) { return left->m_fCost > right->m_fCost; };
	std::priority_queue<PathfindingNode*, std::vector<PathfindingNode*>, decltype(cmp)> openList(cmp);

	startNode->m_parent = nullptr;
	startNode->DeriveHeuristics(startNode, endNode);
	startNode->m_open = true;
	openList.push(startNode);

	PathfindingNode* cursor = startNode;

	while (endNode->m_parent == nullptr)
	{
		std::vector<glm::vec2> adjacentPositions = tilemap->GetAdjacentTilePositions(cursor->m_point);
		for (glm::vec2 pos : adjacentPositions)
		{
			Entity tileEntity = tilemap->GetTile(pos);
			Tile* tile = activeScene->m_entityManager->GetComponent<Tile>(tileEntity);

			// We can't move through non-object tiles so skip em
			if (tile->m_object != TileObject::NONE)
				continue;

			PathfindingNode* node = GetNode(pos);
			if (!node->m_open && !node->m_closed)
			{
				node->m_open = true;
				node->m_parent = cursor;
				node->DeriveHeuristics(startNode, endNode);
				openList.push(node);
			}
		}

		cursor->m_closed = true;

		if (cursor->m_point != end)
		{
			cursor = openList.top();
			openList.pop();
		}
	}

	std::vector<Entity> path;

	while (cursor != nullptr)
	{
		path.insert(path.begin(), tilemap->GetTile(cursor->m_point));
		cursor = cursor->m_parent;
	}

	std::map<std::pair<float, float>, PathfindingNode*>::iterator iterator;
	for (iterator = cache.begin(); iterator != cache.end(); iterator++)
		delete iterator->second;
	cache.clear();

	return path;
}