#include "CameraFollowScript.h"

CameraFollowScript::CameraFollowScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID) :
	Script(entityManager, parentEntityID),
	m_followTarget(playerEntityID),
	m_transform(nullptr)
{}

void CameraFollowScript::OnAttach()
{
	// If no player ID was provided in constructor, try to find player to follow
	if (m_followTarget == 0) { m_followTarget = FindPlayer(); }

	m_transform = GetComponent<Transform>();
	m_camera = GetComponent<Camera>();

	// todo: Make TileLoop here
}

void CameraFollowScript::OnUpdate(float dt)
{
	// If any of the necessary variables are missing, try to find them
	if (m_followTarget == 0)	{ m_followTarget = FindPlayer(); }
	if (!m_camera)				{ m_camera = GetComponent<Camera>(); }
	if (!m_transform)			{ m_transform = GetComponent<Transform>(); }

	if (m_followTarget == 0 || !m_transform || !m_camera) { return; } // If any still do not exist, don't perform follow

	// Get the transform of the follow target
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	Transform* followTransform = entityManager->GetComponent<Transform>(m_followTarget);
	if (!followTransform) { return; }

	float oldPos = followTransform->m_position.x;

	// Transform the camera position. We need to add half the viewport size to center it as the position is in the top-left
	glm::vec2 halfViewportSize(m_camera->m_internalWidth / 2.0f, m_camera->m_internalHeight / 2.0f);
	m_transform->m_position.x = -followTransform->m_position.x + halfViewportSize.x;

	float newPos = m_transform->m_position.x;

	// Get background
	Entity background = 0;

	auto allNameComponents = entityManager->GetAllComponentsOfType<EntityName>();
	for (EntityName* nameComponent : allNameComponents)
	{
		if (nameComponent->m_name == "Background")
		{
			background = entityManager->GetEntityFromComponent<EntityName>(nameComponent);
		}
	}

	Sprite* sprite = entityManager->GetComponent<Sprite>(background);
	sprite->GetShader()->Use();
	sprite->GetShader()->SetUniform("cameraX", followTransform->m_position.x);
	sprite->GetShader()->Unuse();
}

Entity CameraFollowScript::FindPlayer()
{
	Entity player = 0;

	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
	auto allNameComponents = entityManager->GetAllComponentsOfType<EntityName>();
	for (EntityName* nameComponent : allNameComponents)
	{
		if (nameComponent->m_name == "Player")
		{
			player = entityManager->GetEntityFromComponent<EntityName>(nameComponent);
		}
	}

	return player;
}