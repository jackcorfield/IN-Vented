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
	auto it = std::find_if(m_allEntities.begin(), m_allEntities.end(), [&](Entry& e) { return e.entityID == entity; });
	if (it != m_allEntities.end())
		return false;
	
	if (entity == 38)
		int a = 0;
	
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

	m_allEntities.push_back({ entity, transform->m_position });

	return true;
}

bool SpatialHash::remove(Entity entity)
{
	auto it = std::find_if(m_allEntities.begin(), m_allEntities.end(), [&](Entry& e) { return e.entityID == entity; });
	if (it == m_allEntities.end())
		return false;
	
	m_allEntities.erase(it);
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	Transform* transform = ecs->GetComponent<Transform>(entity);
	BoxCollider* boxCollider = ecs->GetComponent<BoxCollider>(entity);

	if (transform == nullptr || boxCollider == nullptr)
	{
		forceRemove(entity);
	}		
	
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

void SpatialHash::forceRemove(Entity entity)
{
	for (auto& pair : m_map)
	{
		if (pair.second.contents.size() == 0)
			continue;
		auto it = std::find(pair.second.contents.begin(), pair.second.contents.end(), entity);
		if (it != pair.second.contents.end())
			pair.second.contents.erase(it);
	}

	auto it = std::find_if(m_allEntities.begin(), m_allEntities.end(), [&](Entry& e) { return e.entityID == entity; });
	if (it != m_allEntities.end())
		m_allEntities.erase(it);
}

void SpatialHash::updateCell(const glm::vec2& cellid)
{
	std::vector<Entity> cellEntities = m_map[cellid].contents;
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	
	for (Entity entity : cellEntities)
	{
		Transform* t = ecs->GetComponent<Transform>(entity);
		auto it = std::find_if(m_allEntities.begin(), m_allEntities.end(), [&](Entry e) { return e.entityID == entity; });

		if (t == nullptr)
		{
			forceRemove(entity);
			continue;
		}
		
		if (it->entryPosition != t->m_position)
		{
			remove(entity);
			insert(entity);
		}
	}
}

void SpatialHash::updateAll()
{
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	for (Entry e : m_allEntities)
	{
		Transform* t = ecs->GetComponent<Transform>(e.entityID);
		if (t == NULL)
		{
			forceRemove(e.entityID);
			continue;
		}
		if (t->m_position != e.entryPosition)
		{
			remove(e.entityID);
			insert(e.entityID);
		}
	}
}

std::vector<Entity> SpatialHash::broadCollisionCheck(Entity entity)
{
	updateAll();
	
	EntityManager* ecs = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	Transform* transform = ecs->GetComponent<Transform>(entity);
	BoxCollider* boxCollider = ecs->GetComponent<BoxCollider>(entity);

	if (transform == nullptr || boxCollider == nullptr)
	{
		forceRemove(entity);
		return {};
	}

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

				if (ecs->GetComponent<Transform>(e) == NULL || ecs->GetComponent<BoxCollider>(e) == NULL)
				{
					forceRemove(e);
					continue;
				}
				entities.push_back(e);
			}
		}
	}

	return entities;
}

bool SpatialHash::isInHash(Entity e) const
{
	return std::find_if(m_allEntities.begin(), m_allEntities.end(), [&](Entry entry) { return entry.entityID == e; }) != m_allEntities.end();
}
