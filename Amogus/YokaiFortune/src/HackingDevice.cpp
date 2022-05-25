#include "HackingDevice.h"

HackingDevice::HackingDevice(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, Entity m_killCounter, int level, bool moving, bool autoTarget) :
	WeaponScript(entityManager, parentEntityID, player, weapon, m_killCounter, level, moving, autoTarget)
{

	m_levelingInfo.push_back(std::make_pair(AREA, 40));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 10));
	m_levelingInfo.push_back(std::make_pair(AREA, 20));
	m_levelingInfo.push_back(std::make_pair(DAMAGE, 3));
	m_levelingInfo.push_back(std::make_pair(AREA, 20));
	m_levelingInfo.push_back(std::make_pair(COOLDOWN, 20));
	m_levelingInfo.push_back(std::make_pair(DAMAGE, 3));

	m_maxLevel = m_levelingInfo.size();

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

	Sprite* icon = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("HackingIcon", "Weapons/HackingDevice/HackingDevice.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	Sprite* sprite = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("HackingAnimation", "Weapons/HackingDevice/Animation/frame1.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));

	SetSprites(icon, sprite);

	Entity newProjectile = m_manager->CreateEntity();

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;

	m_originalTransformSize = glm::vec2(.25f * (m_baseProjectileArea + PercentageIncrease));
	m_manager->AddComponent<Transform>(newProjectile, glm::vec2(1000.0f, 1000.0f), m_originalTransformSize);

	m_manager->AddComponent<Sprite>(newProjectile, m_sprite->GetTexture(), m_sprite->GetColour(), m_sprite->GetShader()); //replace later with animated sprite!

	m_originalBoxSize = glm::vec2(60, 70);
	m_originalBoxOffset = glm::vec2(0.0f, 25.0f);
	m_manager->AddComponent<BoxCollider>(newProjectile, m_originalBoxSize, m_originalBoxOffset); // Needs a box collider that ignores player?

	glm::vec2 direction(0, 0);

	if (m_isMoving)
	{
		direction = currentPosition - m_playerPreviousPosition;
		glm::normalize(direction);
	}

	if (direction == glm::vec2(0, 0))
		direction = glm::vec2(1, 0);

	Projectiles p;

	PercentageIncrease = (m_baseProjectileDuration * m_pScript->m_projectileDuration) / 100;

	p.name = newProjectile;
	p.duration = m_baseProjectileDuration + PercentageIncrease;
	p.direction = direction;
	p.isSpawned = false;

	m_vecProjectiles.push_back(p);
	m_elementNum = m_pScript->AddWeapon(icon, level);
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
		CheckWeaponCollision(m_vecProjectiles[0].name, true);

		float percentageReduction = (m_baseProjectileCooldown * m_pScript->m_projectileCooldown) / 100;
		m_currentCooldown = m_baseProjectileCooldown - percentageReduction;
	}

	float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;

	Transform* transform = m_manager->GetComponent<Transform>(m_vecProjectiles[0].name);
	transform->m_position = currentPosition;
	transform->m_size = m_originalTransformSize * (m_baseProjectileArea + PercentageIncrease);
	
	BoxCollider* boxCollider = m_manager->GetComponent<BoxCollider>(m_vecProjectiles[0].name);
	boxCollider->m_size = m_originalBoxSize* (m_baseProjectileArea + PercentageIncrease);
	boxCollider->m_offset = m_originalBoxOffset* ((m_baseProjectileArea + PercentageIncrease)/2.0f);

	std::cout << PercentageIncrease << std::endl;
	

	m_playerPreviousPosition = currentPosition;
}

void HackingDevice::SpawnProjectile()
{
	
}