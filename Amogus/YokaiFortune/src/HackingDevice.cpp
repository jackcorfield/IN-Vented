#include "HackingDevice.h"

HackingDevice::HackingDevice(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level, bool moving, bool autoTarget) :
	WeaponScript(entityManager, parentEntityID, player, weapon, level, moving, autoTarget)
{
	m_baseProjectileSpeed = 10; //Speed of projectiles

	 //SHOULD BE USED AS THE HITBOX COOLDOWN
	m_baseProjectileCooldown = 1; //How often weapon attacks
	 //SHOULD BE USED AS THE HITBOX COOLDOWN

	m_baseProjectileArea = 2; //Size of weapon
	m_baseProjectileDuration = 0; //How long the projectile stays on the screen
	m_baseProjectileCount = 1; //How many projectiles
	m_projectileMax = 1;

	m_baseDamageModifier = 1;

	m_hitDelay = 1;
	m_critMultiplier = 1;

	m_wait = 0.1;

	m_playerPreviousPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	std::srand(time(NULL));

	Sprite* sprite = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("defaultEntity", "Weapons/HackingDevice/Animation/frame1.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(sprite, sprite);

	Entity newProjectile = m_manager->CreateEntity();

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_manager->AddComponent<Transform>(newProjectile, glm::vec2(1000.0f, 1000.0f), glm::vec2(.25f * m_baseProjectileArea, .25f * m_baseProjectileArea));

	m_manager->AddComponent<Sprite>(newProjectile, m_sprite->GetTexture(), m_sprite->GetColour(), m_sprite->GetShader()); //replace later with animated sprite!
	m_manager->AddComponent<BoxCollider>(newProjectile, transform->m_size); // Needs a box collider that ignores player?

	glm::vec2 direction(0, 0);

	if (m_isMoving)
	{
		direction = currentPosition - m_playerPreviousPosition;
		glm::normalize(direction);
	}

	if (direction == glm::vec2(0, 0))
		direction = glm::vec2(1, 0);

	Projectiles p;

	p.name = newProjectile;
	p.duration = m_baseProjectileDuration;
	p.direction = direction;
	p.isSpawned = false;

	m_vecProjectiles.push_back(p);
}

HackingDevice::~HackingDevice()
{
}

void HackingDevice::OnAttach()
{
}

void HackingDevice::OnRender(float dt)
{
}

void HackingDevice::OnUnattach()
{
}

void HackingDevice::OnUpdate(float dt)
{
	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		//DO HIT HERE!
	}

	
	m_manager->GetComponent<Transform>(m_vecProjectiles[0].name)->m_position = currentPosition;
	m_manager->GetComponent<Transform>(m_vecProjectiles[0].name)->m_size* m_baseProjectileArea;

	m_playerPreviousPosition = currentPosition;
}

void HackingDevice::SpawnProjectile()
{
	//glm::vec2 offset;

	//offset.x = (rand() % 200 - 100) / 2;
	//offset.y = (rand() % 200 - 100) / 2;

	////MAKE BASE SET
	////IF MORE ARE NEEDED (EG THE CURRENT ARE STILL IN USE)
	////ADD MORE ENTITIES TO THE LIST
	////IF NOT, REUSE OLD ENTITIES

	//Projectiles* newProjectile = &m_vecProjectiles[m_currentProjectile];
	//newProjectile->isSpawned = true;

	//m_currentProjectile++;
	//if (m_currentProjectile >= m_vecProjectiles.size() - 1)
	//	m_currentProjectile = 0;

	//Transform* transform = GetComponent<Transform>();

	//glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	//m_manager->GetComponent<Transform>(newProjectile->name)->m_position = currentPosition + offset;

	//glm::vec2 direction(0, 0);



	//if (m_isMoving)
	//{
	//	direction = currentPosition - m_playerPreviousPosition;

	//	if (direction == glm::vec2(0, 0))
	//		direction = glm::vec2(1, 0);
	//	else
	//		direction = glm::normalize(direction);
	//}

	////play noise

	//newProjectile->duration = m_baseProjectileDuration;
	//newProjectile->direction = direction;

}