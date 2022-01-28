#pragma once

#include <cstdint>
#include <map>
#include <typeindex>
#include <utility>
#include <vector>
#include <algorithm>

using Entity = uint32_t;
const Entity MAX_ENTITIES = 65536;

class EntityManager
{
public:
	EntityManager()
		: m_nextEntityID(1)
	{

	}

	~EntityManager()
	{
		m_componentMap.clear();
	}

	Entity CreateEntity()
	{
		if (m_freeIDs.empty() == false)
		{
			Entity id = m_freeIDs[m_freeIDs.size() - 1];
			m_freeIDs.pop_back();
			return id;
		}
		else
		{
			if (m_nextEntityID < MAX_ENTITIES)
			{
				return m_nextEntityID++;
			}
			return 0;
		}
	}

	void DeleteEntity(Entity entity)
	{
		if (entity < MAX_ENTITIES)
		{
			m_freeIDs.push_back(entity);
			for (auto i = m_componentMap.begin(); i != m_componentMap.end(); i++)
			{
				delete i->second[entity];
				i->second.erase(entity);
			}
		}
	}

	bool IsEntity(Entity entity)
	{
		return (entity < m_nextEntityID && std::find(m_freeIDs.begin(), m_freeIDs.end(), entity) == m_freeIDs.end());
	}

	template <typename T, class ...ARGS>
	T* AddComponent(Entity entity, ARGS&&... params)
	{
		if (!IsEntity(entity))
			return nullptr;

		T* component = new T(std::forward<ARGS>(params)...);
		m_componentMap[typeid(T)].insert({ entity, component });

		return component;
	}

	template <typename T>
	bool HasComponent(Entity entity)
	{
		if (!IsEntity(entity))
			return false;

		if (m_componentMap.count(typeid(T)) == 0 || m_componentMap[typeid(T)].count(entity) == 0)
			return false;

		return true;
	}

	template <typename T>
	T* GetComponent(Entity entity)
	{
		return (T*)(m_componentMap[typeid(T)][entity]);
	}

	template <typename T>
	bool RemoveComponent(Entity entity)
	{
		if (!IsEntity(entity))
			return false;

		m_componentMap[typeid(T)].erase(entity);
		return true;
	}

	template <typename T>
	std::vector<T*> GetAllComponentsOfType()
	{
		std::vector<T*> vec;
		for (auto it = m_componentMap[typeid(T)].begin(); it != m_componentMap[typeid(T)].end(); it++)
		{
			vec.push_back((T*)it->second);
		}
		return vec;
	}

	template <typename T>
	Entity GetEntityFromComponent(T* component)
	{
		auto res = std::find_if(m_componentMap[typeid(T)].begin(), m_componentMap[typeid(T)].end(), [&](const std::pair<Entity, void*>& param)
			{
				return param.second == (void*)component;
			});

		if (res != m_componentMap[typeid(T)].end())
		{
			return res->first;
		}
		return 0;
	}

	std::vector<Entity> GetAllActiveEntities()
	{
		std::vector<Entity> ids;

		for (int i = 1; i < m_nextEntityID; i++)
		{
			if (std::find(m_freeIDs.begin(), m_freeIDs.end(), i) == m_freeIDs.end())
			{
				ids.push_back(i);
			}
		}

		return ids;
	}

	std::map<std::type_index, void*> GetAllComponents(Entity id)
	{
		std::map<std::type_index, void*> componentMap;

		for (std::pair<std::type_index, std::map<Entity, void*>> i : m_componentMap)
		{
			auto res = std::find_if(i.second.begin(), i.second.end(), [&](const std::pair<Entity, void*>& param)
				{
					return param.first == id;
				});

			if (res != i.second.end())
			{
				componentMap[i.first] = res->second;
			}
		}

		return componentMap;
	}

private:
	unsigned int m_nextEntityID;
	std::vector<Entity> m_freeIDs;

	std::map<std::type_index, std::map<Entity, void*>> m_componentMap;
};