#include "PlayerScript.h"

PlayerScript::PlayerScript(EntityManager* entityManager, Entity parentEntityID, float speed):Script(entityManager, parentEntityID), 
//m_manager(entityManager),
m_movementSpeed(speed),
//m_player(parentEntityID),
m_leftStickDirection(glm::vec2(0,0)),
m_leftStickInUse(false)
{
	InputHandler::GetMapping("Input_Movement")->m_bus->subscribe(this, &PlayerScript::KeyEvent);

	m_keyboardInUse = false;

	m_registeredKeys.W = false;
	m_registeredKeys.A = false;
	m_registeredKeys.S = false;
	m_registeredKeys.D = false;

	m_registeredKeys.UP = false;
	m_registeredKeys.DOWN = false;
	m_registeredKeys.LEFT = false;
	m_registeredKeys.RIGHT = false;
}

PlayerScript::~PlayerScript()
{}

void PlayerScript::OnAttach()
{
	g_app->m_debugger->Log("ATTACHED", LL_DEBUG);
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

		switch (e->m_gamepadInput)
		{
		case XB1_DPAD_UP:
			m_registeredKeys.UP = true;
			break;
		case XB1_DPAD_DOWN:
			m_registeredKeys.DOWN = true;
			break;
		case XB1_DPAD_LEFT:
			m_registeredKeys.LEFT = true;
			break;
		case XB1_DPAD_RIGHT:
			m_registeredKeys.RIGHT = true;
			break;
		default:
			break;
		}

		m_leftStickInUse = false;
		//m_latestGameInput = e->m_gamepadInput;
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
		//m_latestGameInput = -1;
	}
	else if(e->m_leftStickAxis.x < 0.1f && e->m_leftStickAxis.y < 0.1f && e->m_leftStickAxis.x > -0.1f && e->m_leftStickAxis.y > -0.1f)
	{
		m_leftStickInUse = false;
	}
	else if(e->m_leftStickAxis != glm::vec2(-1.0f, -1.0f))
	{
		m_leftStickDirection = e->m_leftStickAxis;
		m_leftStickInUse = true;
	}

}

void PlayerScript::OnUpdate(float dt)
{
	//if (!m_manager || m_player == 0)
		//return;

	Transform* playerTransform = GetComponent<Transform>();

	// Use to check if these properties change this frame
	bool moving = false;
	bool facingLeft = m_facingLeftLastFrame;

	//Keyboard Controls
	if (m_registeredKeys.W)
	{
		playerTransform->m_position.y -= m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
	}
	if (m_registeredKeys.D)
	{
		playerTransform->m_position.x += m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
		facingLeft = false;
	}
	if (m_registeredKeys.S)
	{
		playerTransform->m_position.y += m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
	}
	if (m_registeredKeys.A)
	{
		playerTransform->m_position.x -= m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
		facingLeft = true;
	}
		

	//Gamepad Controls
	if (m_registeredKeys.UP && !m_keyboardInUse)
	{
		playerTransform->m_position.y -= m_movementSpeed * dt;
		m_registeredKeys.UP = false;
		moving = true;
	}
	if (m_registeredKeys.RIGHT && !m_keyboardInUse)
	{
		playerTransform->m_position.x += m_movementSpeed * dt;
		m_registeredKeys.RIGHT = false;
		moving = true;
		facingLeft = false;
	}
	if (m_registeredKeys.DOWN && !m_keyboardInUse)
	{
		playerTransform->m_position.y += m_movementSpeed * dt;
		m_registeredKeys.DOWN = false;
		moving = true;
	}
	if (m_registeredKeys.LEFT && !m_keyboardInUse)
	{
		playerTransform->m_position.x -= m_movementSpeed * dt;
		m_registeredKeys.LEFT = false;
		moving = true;
		facingLeft = true;
	}

	//Joystick Control
	if (m_leftStickInUse)
	{
		playerTransform->m_position.x += m_leftStickDirection.x * m_movementSpeed * dt;
		playerTransform->m_position.y += m_leftStickDirection.y * m_movementSpeed * dt;
		moving = true;

		facingLeft = false;
		if (m_leftStickDirection.x < 0)
		{
			facingLeft = true;
		}
	}

	UpdateSpriteAnimation(facingLeft, moving);

	// Update member based on current frame
	m_keyboardInUse = false;
}

void PlayerScript::OnRender(float dt)
{

}

void PlayerScript::OnUnattach()
{

}

void PlayerScript::UpdateSpriteAnimation(bool facingLeft, bool moving)
{
	AnimatedSprite* animatedSprite = GetComponent<AnimatedSprite>();

	// If changed direction, change running sprite. Turning means we are moving anyway so no need to check
	if (facingLeft && !m_facingLeftLastFrame)
	{
		animatedSprite->setAnimation("RunLeft");
	}
	else if (!facingLeft && m_facingLeftLastFrame)
	{
		animatedSprite->setAnimation("RunRight");
	}

	// If newly moving, start running
	if (moving && !m_movingLastFrame)
	{
		// Choose animation based on direction
		if (facingLeft)
		{
			animatedSprite->setAnimation("RunLeft");
		}
		else
		{
			animatedSprite->setAnimation("RunRight");
		}

	}
	else if (!moving && m_movingLastFrame) // If newly idle, start idle animation
	{
		facingLeft = false; // Default direction is right
		animatedSprite->setAnimation("Idle");
	}

	// Update members based on current frame
	m_movingLastFrame = moving;
	m_facingLeftLastFrame = facingLeft;
}