#pragma once

#include <glm.hpp>

#include "EntityManager.h"

class Scene
{
public:
	Scene();
	Scene(const glm::vec3& sceneColour);

	~Scene();


	glm::vec3 m_sceneColour;
	EntityManager* m_entityManager;
};