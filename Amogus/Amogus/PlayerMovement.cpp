#include "PlayerMovement.h"

#include "source.h"
#include "Physics.h"

#include <iostream>

extern Application* g_app;

PlayerMovement::PlayerMovement()
	: m_speed(10000.0f)
{
	InputHandler::GetMapping("Input_Movement")->m_bus->subscribe(this, &PlayerMovement::AddMovement);
}

void PlayerMovement::AddMovement(InputEvent* e)
{
	Scene* activeScene = g_app->m_sceneManager->GetActiveScene();
	if (!activeScene) return;

	Entity thisEntity = activeScene->m_entityManager->GetEntityFromComponent<PlayerMovement>(this);
	Physics* physics = activeScene->m_entityManager->GetComponent<Physics>(thisEntity);

	if (thisEntity == NULL || physics == nullptr)
		return;

	if (e->m_keyInput != -1)
	{
		switch (e->m_keyInput)
		{
		case (GLFW_KEY_W):
			physics->AddForce(glm::vec2(0, -1 * m_speed));
			std::cout << "MOVING UP PLEASE" << std::endl;
			break;
		case (GLFW_KEY_A):
			physics->AddForce(glm::vec2(-1 * m_speed, 0));
			break;
		case (GLFW_KEY_S):
			physics->AddForce(glm::vec2(0, 1 * m_speed));
			break;
		case (GLFW_KEY_D):
			physics->AddForce(glm::vec2(1 * m_speed, 0));
			break;
		}
	}
	else if (e->m_gamepadInput != -1)
	{
		switch (e->m_gamepadInput)
		{
		case (XB1_DPAD_UP):
			physics->AddForce(glm::vec2(0, -1 * m_speed));
			std::cout << "MOVING UP PLEASE" << std::endl;
			break;
		case (XB1_DPAD_LEFT):
			physics->AddForce(glm::vec2(-1 * m_speed, 0));
			break;
		case (XB1_DPAD_DOWN):
			physics->AddForce(glm::vec2(0, 1 * m_speed));
			break;
		case (XB1_DPAD_RIGHT):
			physics->AddForce(glm::vec2(1 * m_speed, 0));
			break;
		}
	}
}
