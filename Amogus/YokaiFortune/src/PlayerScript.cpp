#include "PlayerScript.h"
// While it seems wrong to hard set viewport values, these are integral to gameplay anyway and should be constant
#define MAX_Y 180.0f
#define MIN_Y -180.0f

PlayerScript::PlayerScript(EntityManager* entityManager, Entity parentEntityID, Entity GameOver, float speed) :
	Script(entityManager, parentEntityID),
	m_transform(nullptr),
	m_collider(nullptr),
	m_keyboardInUse(false),
	m_registeredKeys(),
	m_leftStickInUse(false),
	m_leftStickDirection(glm::vec2(0.0f)),
	m_movingLastFrame(false),
	m_facingLeftLastFrame(false),
	m_invulnTime(0.1f),
	m_currentInvulnCooldown(0.0f),
	m_isDead(false),
	m_movementSpeed(speed),
	m_projectileSpeed(10),
	m_projectileCount(0),
	m_projectileCooldown(1),
	m_projectileArea(10),
	m_projectileDuration(10),
	m_damageModifier(1.0f),
	m_maxHealth(36.0f),
	m_health(m_maxHealth),
	m_playerXP(0),
	m_levelUpXP(500),
	m_regeneration(1.0f),
	m_range(50.0f)
{
	InputHandler::GetMapping("Input_Movement")->m_bus->subscribe(this, &PlayerScript::KeyEvent);

	m_needLevel = false;

	m_registeredKeys.W = false;
	m_registeredKeys.A = false;
	m_registeredKeys.S = false;
	m_registeredKeys.D = false;

	m_registeredKeys.UP = false;
	m_registeredKeys.DOWN = false;
	m_registeredKeys.LEFT = false;
	m_registeredKeys.RIGHT = false;
	
	m_UIWidget = entityManager->GetComponent<UI_WidgetComponent>(parentEntityID);
	m_GameOver = entityManager->GetComponent<UI_WidgetComponent>(GameOver);
	
}

PlayerScript::~PlayerScript()
{}

void PlayerScript::OnAttach()
{
	m_transform = GetComponent<Transform>();
	m_collider = GetComponent<BoxCollider>();
}

void PlayerScript::AddXP(int XPVal)
{
	m_playerXP += XPVal;

	// Level up?
	if (m_playerXP >= m_levelUpXP)
	{
		m_needLevel = true;
		m_playerXP = m_playerXP - m_levelUpXP;
		m_levelUpXP *= 2;
	}
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
	if (m_isDead) 
	{ 
		for (UI_BaseElement* element : m_GameOver->m_elements)
		{
			element->m_hidden = false;
		}

		return;
	}

	if (m_currentInvulnCooldown > 0.0f)
	{
		m_currentInvulnCooldown -= dt;
	}

	m_health += m_regeneration * dt;
	if (m_health > m_maxHealth)
	{
		m_health = m_maxHealth;
	}

	// Use to check if these properties change this frame
	bool moving = false;
	bool facingLeft = m_facingLeftLastFrame;

	//Keyboard Controls
	if (m_registeredKeys.W)
	{
		m_transform->m_position.y -= m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
	}
	if (m_registeredKeys.D)
	{
		m_transform->m_position.x += m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
		facingLeft = false;
	}
	if (m_registeredKeys.S)
	{
		m_transform->m_position.y += m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
	}
	if (m_registeredKeys.A)
	{
		m_transform->m_position.x -= m_movementSpeed * dt;
		m_keyboardInUse = true;
		moving = true;
		facingLeft = true;
	}

	//Gamepad Controls
	if (m_registeredKeys.UP && !m_keyboardInUse)
	{
		m_transform->m_position.y -= m_movementSpeed * dt;
		m_registeredKeys.UP = false;
		moving = true;
	}
	if (m_registeredKeys.RIGHT && !m_keyboardInUse)
	{
		m_transform->m_position.x += m_movementSpeed * dt;
		m_registeredKeys.RIGHT = false;
		moving = true;
		facingLeft = false;
	}
	if (m_registeredKeys.DOWN && !m_keyboardInUse)
	{
		m_transform->m_position.y += m_movementSpeed * dt;
		m_registeredKeys.DOWN = false;
		moving = true;
	}
	if (m_registeredKeys.LEFT && !m_keyboardInUse)
	{
		m_transform->m_position.x -= m_movementSpeed * dt;
		m_registeredKeys.LEFT = false;
		moving = true;
		facingLeft = true;
	}

	//Joystick Control
	if (m_leftStickInUse)
	{
		m_transform->m_position.x += m_leftStickDirection.x * m_movementSpeed * dt;
		m_transform->m_position.y += m_leftStickDirection.y * m_movementSpeed * dt;
		moving = true;

		facingLeft = false;
		if (m_leftStickDirection.x < 0)
		{
			facingLeft = true;
		}
	}

	if (m_transform->m_position.y > MAX_Y)
	{
		m_transform->m_position.y = MAX_Y;
	}
	else if (m_transform->m_position.y < MIN_Y)
	{
		m_transform->m_position.y = MIN_Y;
	}

	UpdateSpriteAnimation(facingLeft, moving);

	CheckCollisions();

	// Update member based on current frame
	m_keyboardInUse = false;
}

void PlayerScript::OnRender(float dt)
{

}

void PlayerScript::OnUnattach()
{

}

int PlayerScript::AddWeapon(Sprite* icon, int level)
{
	if (m_weaponCount < 5)
	{
		UI_Image* image = (UI_Image*)m_UIWidget->m_elements[m_weaponCount];
		image->m_texture = icon->GetTexture();

		UpdateLevel(m_weaponCount, level+1);
		m_weaponCount++;
	}
	else
		return NULL;

	return m_weaponCount;
}

int PlayerScript::AddEquip(Sprite* icon, int level)
{
	if (m_equipCount < 10)
	{
		UI_Image* image = (UI_Image*)m_UIWidget->m_elements[m_equipCount];
		image->m_texture = icon->GetTexture();

		UpdateLevel(m_equipCount, level);
		m_equipCount++;
	}
	else
		return NULL;

	return m_equipCount;
}

void PlayerScript::UpdateSpriteAnimation(bool facingLeft, bool moving)
{
	AnimatedSprite* animatedSprite = GetComponent<AnimatedSprite>();

	bool changedDirection = facingLeft != m_facingLeftLastFrame;
	if (changedDirection) // If changed direction, reverse size
	{
		m_transform->m_size.x = -m_transform->m_size.x;
	}

	if (moving && !m_movingLastFrame) // If newly moving, start running
	{
		animatedSprite->setAnimation("Run");

	}
	else if (!moving && m_movingLastFrame) // If newly idle, start idle animation
	{
		animatedSprite->setAnimation("Idle");
	}

	// Update members based on current frame
	m_movingLastFrame = moving;
	m_facingLeftLastFrame = facingLeft;
}

void PlayerScript::CheckCollisions()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	std::vector<Entity> allPossibleCollisions = GetNearbyEntities();
	for (Entity possibleCollision : allPossibleCollisions)
	{
		bool intersecting = CheckPotentialCollision(possibleCollision);
		if (!intersecting) { continue; }

		glm::vec2 intersection(GetIntersectionDepth(possibleCollision));

		BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(possibleCollision);
		Transform* theirTransform = entityManager->GetComponent<Transform>(possibleCollision);
		ResolveCollision(intersection, theirCollider, theirTransform);
	}
}

bool PlayerScript::CheckPotentialCollision(Entity possibleCollision)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Check that this is an enemy
	EntityName* name = entityManager->GetComponent<EntityName>(possibleCollision);
	if (!name)
		return false;
	if (name->m_name != "Enemy") { return false; }

	// Get relevant components
	BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(possibleCollision);
	Transform* theirTransform = entityManager->GetComponent<Transform>(possibleCollision);

	// Calculate these for convenience
	glm::vec2 myHalfSize = m_collider->m_size / 2.0f;
	float myTop = m_transform->m_position.y - myHalfSize.y + m_collider->m_offset.y;
	float myBottom = m_transform->m_position.y + myHalfSize.y + m_collider->m_offset.y;
	float myLeft = m_transform->m_position.x - myHalfSize.x + m_collider->m_offset.x;
	float myRight = m_transform->m_position.x + myHalfSize.x + m_collider->m_offset.x;

	glm::vec2 theirHalfSize = theirCollider->m_size / 2.0f;
	float theirTop = theirTransform->m_position.y - theirHalfSize.y + theirCollider->m_offset.y;
	float theirBottom = theirTransform->m_position.y + theirHalfSize.y + theirCollider->m_offset.y;
	float theirLeft = theirTransform->m_position.x - theirHalfSize.x + theirCollider->m_offset.x;
	float theirRight = theirTransform->m_position.x + theirHalfSize.x + theirCollider->m_offset.x;

	// Quick check for collision
	if (myRight < theirLeft || myLeft > theirRight || myBottom < theirTop || myTop > theirBottom)
	{
		return false; // No collision
	}

	return true;
}

glm::vec2 PlayerScript::GetIntersectionDepth(Entity collidedEntity)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Get relevant components
	BoxCollider* theirCollider = entityManager->GetComponent<BoxCollider>(collidedEntity);
	Transform* theirTransform = entityManager->GetComponent<Transform>(collidedEntity);

	glm::vec2 myPos = m_transform->m_position + m_collider->m_offset;
	glm::vec2 theirPos = theirTransform->m_position + theirCollider->m_offset;

	float wy = (m_collider->m_size.x + theirCollider->m_size.x) * (myPos.y - theirPos.y);
	float hx = (m_collider->m_size.y + theirCollider->m_size.y) * (myPos.x - theirPos.x);

	glm::vec2 depth;
	if (wy > hx)
	{
		if (wy > -hx) // Top
		{
			depth.y = -1;
		}
		else // Left
		{
			depth.x = 1;
		}
	}
	else
	{
		if (wy > -hx) // Right
		{
			depth.x = -1;
		}
		else // Bottom
		{
			depth.y = 1;
		}
	}

	return depth;
}

void PlayerScript::UpdateLevel(int elementNum, int num)
{
	UI_Text* text;

	if(elementNum == 10)
		text = (UI_Text*)m_UIWidget->m_elements[19];
	else
		text = (UI_Text*) m_UIWidget->m_elements[elementNum + 10];

	text->m_text = std::to_string(num);
}

void PlayerScript::ResolveCollision(glm::vec2 intersection, BoxCollider* theirCollider, Transform* theirTransform)
{
	glm::vec2 theirHalfSize = theirCollider->m_size / 2.0f;

	float theirTop = theirTransform->m_position.y - theirHalfSize.y + theirCollider->m_offset.y;
	float theirBottom = theirTransform->m_position.y + theirHalfSize.y + theirCollider->m_offset.y;
	float theirLeft = theirTransform->m_position.x - theirHalfSize.x + theirCollider->m_offset.x;
	float theirRight = theirTransform->m_position.x + theirHalfSize.x + theirCollider->m_offset.x;

	glm::vec2 myHalfSize = GetComponent<BoxCollider>()->m_size / 2.0f;

	if (intersection.y == 1) // Bottom
	{
		m_transform->m_position.y = theirTop - myHalfSize.y - m_collider->m_offset.y;
	}
	else if (intersection.y == -1) // Top
	{
		m_transform->m_position.y = theirBottom + myHalfSize.y - m_collider->m_offset.y;
	}

	if (intersection.x == 1) // Left
	{
		m_transform->m_position.x = theirLeft - myHalfSize.x - m_collider->m_offset.x;
	}
	else if (intersection.x == -1) // Right
	{
		m_transform->m_position.x = theirRight + myHalfSize.x - m_collider->m_offset.x;
	}

	// Do damage to player if invuln cooldown is off
	if (m_currentInvulnCooldown <= 0.0f)
	{
		// Do damage
		m_health -= 1.0f; // Placeholder number
		if (m_health <= 0.0f)
		{
			Audio* audio = AddComponent<Audio>("sfx/death.wav", g_app->m_audioManager->m_system, g_app->m_audioManager->m_sfx);
			g_app->m_audioManager->PlayAudio(audio->m_sound, audio->m_group, audio->m_channel);

			m_isDead = true;
		}
		m_currentInvulnCooldown = m_invulnTime;
	}
}