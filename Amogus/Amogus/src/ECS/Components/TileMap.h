#pragma once

#include <glm/glm.hpp>
#include <map>

#include <ECS/EntityManager.h>
#include <Core/source.h>

extern Application* g_app;

class TileMap
{
public:
	TileMap(const glm::vec2& tileSize, const glm::vec2& mapSize, const std::vector<Entity>& tiles)
		: m_tileSize(tileSize), m_mapSize(mapSize)
	{

		// This assumes that the tiles vector is laid out from left to right, top to bottom, and that all tiles are adjacent to each other

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

	glm::vec2 ConvertPositionToTileIndex(const glm::vec2& position)
	{
		Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
		Entity tilemapEntity = activeScene->m_entityManager->GetEntityFromComponent<TileMap>(this);
		Transform* transform = activeScene->m_entityManager->GetComponent<Transform>(tilemapEntity);

		glm::vec2 tilePos;
		tilePos.y = position.y - position.x / 2.0f - m_tileSize.y;
		tilePos.x = position.x + tilePos.y;
		tilePos -= transform->m_position;

		return glm::vec2(ceilf(tilePos.x / m_tileSize.x) + 1, ceilf(-tilePos.y / m_tileSize.x));
	}

	Entity GetTileAtPosition(const glm::vec2& position)
	{
		return GetTile(ConvertPositionToTileIndex(position));
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

	std::vector<glm::vec2> GetAdjacentTilePositions(const glm::vec2& centre, bool diagonal = false)
	{
		std::vector<glm::vec2> positions;

		/*
		[-1,1] [0,1] [1,1]
		[-1,0] [0,0] [1,0]
		[-1,-1][0,-1][1,-1] 
		*/
		for (int x = -1; x < 1; x++)
		{
			for (int y = -1; y < 1; y++)
			{
				// Ignore self
				if (x == 0 && y == 0)
					continue;

				// Ignore diagonal tiles if we don't want them
				if (x != 0 && y != 0 && !diagonal)
					continue;

				// Ignore invalid tiles
				glm::vec2 position = glm::vec2(centre.x + x, centre.y + y);

				if (position.x < 0 || position.x > m_tileSize.x)
					continue;

				if (position.y < 0 || position.y > m_tileSize.y)
					continue;

				positions.push_back(position);
			}
		}

		return positions;
	}

	glm::vec2 m_tileSize;
	glm::vec2 m_mapSize;

private:
	// std::map doesn't like glm::vec2 being used as a key :(
	std::map<std::pair<float, float>, Entity> m_tileMap;
};