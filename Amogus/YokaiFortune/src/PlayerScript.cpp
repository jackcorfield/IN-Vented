#include "PlayerScript.h"

PlayerScript::PlayerScript(EntityManager* entityManager, Entity parentEntityID, float speed):Script(entityManager, parentEntityID), 
m_manager(entityManager),
m_movementSpeed(speed),
m_player(parentEntityID),
m_movePlayer(false)
{
	InputHandler::GetMapping("Input_Movement")->m_bus->subscribe(this, &PlayerScript::KeyEvent);

	m_registeredKeys.W = false;
	m_registeredKeys.A = false;
	m_registeredKeys.S = false;
	m_registeredKeys.D = false;
}

void PlayerScript::OnAttach()
{
	g_app->m_debugger->Log("ATTACHED", LL_ERROR);
}

void PlayerScript::KeyEvent(InputEvent* e)
{
	if (e->m_action == GLFW_PRESS)
	{
		switch (e->m_keyInput)
		{
		case GLFW_KEY_W:
			m_registeredKeys.W = true;
			break;
		case GLFW_KEY_A:
			m_registeredKeys.A = true;
			//AnimatedSprite* pAnimatedSprite = GetComponent<AnimatedSprite>();
			//pAnimatedSprite->SetFrames();
			break;
		case GLFW_KEY_S:
			m_registeredKeys.S = true;
			break;
		case GLFW_KEY_D:
			m_registeredKeys.D = true;
			break;

		default:
			break;
		}


		m_latestGameInput = e->m_gamepadInput;
	}
	else if (e->m_action == GLFW_RELEASE)
	{
		switch (e->m_keyInput)
		{
		case GLFW_KEY_W:
			m_registeredKeys.W = false;
			break;
		case GLFW_KEY_A:
			m_registeredKeys.A = false;
			break;
		case GLFW_KEY_S:
			m_registeredKeys.S = false;
			break;
		case GLFW_KEY_D:
			m_registeredKeys.D = false;
			break;

		default:
			break;
		}
		m_latestGameInput = -1;
	}
}

void PlayerScript::OnUpdate(float dt)
{
	if (!m_manager || m_player == 0)
		return;

	Transform* playerTransform = GetComponent<Transform>();

	if(m_registeredKeys.W)
		playerTransform->m_position.y -= m_movementSpeed * dt;
	if (m_registeredKeys.D)
		playerTransform->m_position.x += m_movementSpeed * dt;
	if (m_registeredKeys.S)
		playerTransform->m_position.y += m_movementSpeed * dt;
	if (m_registeredKeys.A)
		playerTransform->m_position.x -= m_movementSpeed * dt;

	/*switch (m_latestGameInput)
	{
	case (XB1_DPAD_UP):
		playerTransform->m_position.y += m_speed * dt;
		break;
	case (XB1_DPAD_RIGHT):
		playerTransform->m_position.x += m_speed * dt;
		break;
	case (XB1_DPAD_DOWN):
		playerTransform->m_position.y -= m_speed * dt;
		break;
	case (XB1_DPAD_LEFT):
		playerTransform->m_position.x -= m_speed * dt;
		break;
	default:
		break;
	}*/

}

void PlayerScript::OnRender(float dt)
{

}

void PlayerScript::OnUnattach()
{

}

PlayerScript::~PlayerScript()
{

}