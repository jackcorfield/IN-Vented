#pragma once

#include <glm.hpp>
#include <map>

#include "EntityManager.h"

class TileMap
{
public:
	TileMap(const glm::vec2& tileSize, const glm::vec2& mapSize, const std::vector<Entity>& tiles)
		: m_tileSize(tileSize), m_mapSize(mapSize)
	{
		int it = 0;
		for (int i = 0; i < mapSize.y; i++)
		{
			for (int j = 0; j < mapSize.x; j++)
			{
				m_tileMap[std::pair<float, float>(j, i)] = tiles[it++];
			}
		}
	}

	~TileMap()
	{

	}

	Entity GetTile(const glm::vec2& tileIndex)
	{
		if (m_tileMap.count(std::pair<float, float>(tileIndex.x, tileIndex.y)) != 0)
			return m_tileMap[std::pair<float, float>(tileIndex.x, tileIndex.y)];
		return 0;
	}

	Entity GetTileAtPosition(const glm::vec2& position)
	{
		glm::vec2 tilePos;
		tilePos.y = position.y - position.x / 2.0f - m_tileSize.y;
		tilePos.x = position.x + tilePos.y;

		glm::vec2 tileIndex(ceilf(tilePos.x / m_tileSize.x) + 1, ceilf(-tilePos.y / m_tileSize.x));
		return GetTile(tileIndex);
	}

	std::vector<Entity> GetAllAdjacentTiles(const glm::vec2& tileIndex, bool diagonal = false)
	{
		std::vector<Entity> tiles;

		if (diagonal)
		{
			tiles.push_back(GetTile(glm::vec2(tileIndex.x - 1, tileIndex.y - 1)));
			tiles.push_back(GetTile(glm::vec2(tileIndex.x + 1, tileIndex.y - 1)));
			tiles.push_back(GetTile(glm::vec2(tileIndex.x - 1, tileIndex.y + 1)));
			tiles.push_back(GetTile(glm::vec2(tileIndex.x + 1, tileIndex.y + 1)));
		}

		tiles.push_back(GetTile(glm::vec2(tileIndex.x,     tileIndex.y - 1)));
		tiles.push_back(GetTile(glm::vec2(tileIndex.x - 1, tileIndex.y)));
		tiles.push_back(GetTile(glm::vec2(tileIndex.x + 1, tileIndex.y)));
		tiles.push_back(GetTile(glm::vec2(tileIndex.x,     tileIndex.y + 1)));

		return tiles;
	}

	glm::vec2 m_tileSize;
	glm::vec2 m_mapSize;

private:
	std::map<std::pair<float, float>, Entity> m_tileMap;
};