#include "Scene.h"

Scene::Scene()
	: Scene(glm::vec3(0.0f))
{
}

Scene::Scene(const glm::vec3& sceneColour)
	: m_sceneColour(sceneColour),
	m_entityManager(new EntityManager)
{
}

Scene::~Scene()
{
	delete m_entityManager;
	m_entityManager = nullptr;
}