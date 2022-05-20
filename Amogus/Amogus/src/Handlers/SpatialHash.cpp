#include "SpatialHash.h"

#include <ECS/Components/BoxCollider.h>
#include <ECS/Components/Transform.h>

#include <Core/source.h>

extern Application* g_app;

SpatialHash::SpatialHash(unsigned int cellSize)
	: m_cellSize(cellSize) { }

glm::vec2 SpatialHash::hash(const glm::vec2& position)
{
	return glm::vec2((int)position.x / (int)m_cellSize, (int)position.y / (int)m_cellSize);
}

bool SpatialHash::insert(Entity entity)
{
	if (std::find(m_allEntities.begin(), m_allEntities.end(), entity) != m_allEntities.end())
		return false;
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	
	Transform* transform = ecs->GetComponent<Transform>(entity);
	BoxCollider* boxCollider = ecs->GetComponent<BoxCollider>(entity);

	if (transform == nullptr || boxCollider == nullptr)
		return false;

	glm::vec2 min = hash(transform->m_position - boxCollider->m_size / 2.0f);
	glm::vec2 max = hash(transform->m_position + boxCollider->m_size / 2.0f);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			m_map[glm::vec2(x, y)].contents.push_back(entity);
		}
	}

	m_allEntities.push_back(entity);

	return true;
}

bool SpatialHash::remove(Entity entity)
{
	auto iter = std::find(m_allEntities.begin(), m_allEntities.end(), entity);
	if (iter == m_allEntities.end())
		return false;
	
	m_allEntities.erase(iter);
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	Transform* transform = ecs->GetComponent<Transform>(entity);
	BoxCollider* boxCollider = ecs->GetComponent<BoxCollider>(entity);

	if (transform == nullptr || boxCollider == nullptr)
		return false;
	
	glm::vec2 min = hash(transform->m_position - boxCollider->m_size / 2.0f);
	glm::vec2 max = hash(transform->m_position + boxCollider->m_size / 2.0f);
	
	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			std::vector<Entity>& contents = m_map[glm::vec2(x, y)].contents;
			auto iter = std::find(contents.begin(), contents.end(), entity);
			if (iter != contents.end())
				contents.erase(iter);
		}
	}

	return true;
}

void SpatialHash::updateCell(const glm::vec2& cellid)
{
	std::vector<Entity> cellEntities = m_map[cellid].contents;
	for (Entity entity : cellEntities)
	{
		remove(entity);
		insert(entity);
	}
}

void SpatialHash::updateAll()
{
	for (Entity e : m_allEntities)
	{
		remove(e);
		insert(e);
	}
}

std::vector<Entity> SpatialHash::broadCollisionCheck(Entity entity)
{
	updateAll();
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	Transform* transform = ecs->GetComponent<Transform>(entity);
	BoxCollider* boxCollider = ecs->GetComponent<BoxCollider>(entity);

	if (transform == nullptr || boxCollider == nullptr)
		return {};

	glm::vec2 min = hash(transform->m_position - boxCollider->m_size / 2.0f);
	glm::vec2 max = hash(transform->m_position + boxCollider->m_size / 2.0f);
	
	std::vector<Entity> entities;

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			std::vector<Entity>& contents = m_map[glm::vec2(x, y)].contents;
			
			for (Entity e : contents)
			{
				if (e == entity)
					continue;
				entities.push_back(e);
			}
		}
	}

	return entities;
}

bool SpatialHash::isInHash(Entity e) const
{
	return std::find(m_allEntities.begin(), m_allEntities.end(), e) != m_allEntities.end();
}
