#pragma once

#include <glm/glm.hpp>

#include <ECS/EntityManager.h>

class Scene
{
public:
	Scene();
	Scene(const glm::vec3& sceneColour);

	~Scene();


	glm::vec3 m_sceneColour;
	EntityManager* m_entityManager;

};