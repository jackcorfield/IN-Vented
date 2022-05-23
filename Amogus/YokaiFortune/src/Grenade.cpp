#include "Grenade.h"

Grenade::Grenade(EntityManager* entityManager, Entity parentEntityID, Entity player, Entity weapon, int level, bool moving, bool autoTarget) :
	WeaponScript(entityManager, parentEntityID, player, weapon, level, moving, autoTarget)
{
	m_baseProjectileSpeed = 0.2f; //Speed of projectiles
	m_baseProjectileCooldown = 3; //How often weapon attacks
	m_baseProjectileArea = 1; //Size of weapon
	m_baseProjectileDuration = 4; //How long the projectile stays on the screen
	m_baseProjectileCount = 1; //How many projectiles
	m_projectileMax = 10;

	m_baseDamageModifier = 1;

	m_hitDelay = 1;
	m_critMultiplier = 1;

	m_wait = 0.1;

	m_playerPreviousPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	std::srand(time(NULL));

	Sprite* sprite = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("GrenadeIconSprite", "Weapons/Grenade/grenade.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	SetSprites(sprite, sprite);

	//AnimatedSprite* aSprite = entityManager->AddComponent<AnimatedSprite>(weapon, TextureLoader::CreateTexture2DFromFile("GrenadeIconSprite", "Weapons/Grenade/GrenadeSpriteSheet.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
	AnimatedSprite* templateASprite = GetComponent<AnimatedSprite>();

	m_audio = entityManager->AddComponent<Audio>(weapon, "sfx/Weapons/shuriken.mp3", g_app->m_audioManager->m_system, g_app->m_audioManager->m_sfx);
	m_vecProjectiles.resize(m_projectileMax);

	for (int i = 0; i < m_projectileMax; i++)
	{
		Entity newProjectile = m_manager->CreateEntity();

		Transform* transform = GetComponent<Transform>();

		glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

		float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileArea) / 100;
		m_manager->AddComponent<Transform>(newProjectile, glm::vec2(1000.0f, 1000.0f), glm::vec2(.25f * (m_baseProjectileArea + PercentageIncrease), .25f * m_baseProjectileArea + PercentageIncrease));

		AnimatedSprite* newASprite = m_manager->AddComponent<AnimatedSprite>(newProjectile, templateASprite->GetTexture(), templateASprite->getFrameSize(), templateASprite->GetColour(), templateASprite->GetShader());
		//m_manager->AddComponent<Sprite>(newProjectile, m_sprite->GetTexture(), m_sprite->GetColour(), m_sprite->GetShader()); //replace later with animated sprite!
		m_manager->AddComponent<BoxCollider>(newProjectile, transform->m_size, glm::vec2(0.0f)); // Needs a box collider that ignores player?

		// Set up animations
		std::map<std::string, Animation> animations = templateASprite->getAnimations();
		for (auto animationItr = animations.begin(); animationItr != animations.end(); animationItr++)
		{
			std::string name = animationItr->first;
			float frameTime = animationItr->second.frameTime;
			std::vector<unsigned int> frames = animationItr->second.frames;
			newASprite->createAnimation(name, frames, frameTime);
		}
		newASprite->setAnimation("Throwing");

		glm::vec2 direction(0, 0);

		if (m_isMoving)
		{
			direction = currentPosition - m_playerPreviousPosition;
			glm::normalize(direction);
		}

		if (direction == glm::vec2(0, 0))
			direction = glm::vec2(1, 0);

		Projectiles p;

		PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileDuration) / 100;

		p.name = newProjectile;
		p.duration = m_baseProjectileDuration + PercentageIncrease;
		p.direction = direction;
		p.isSpawned = false;
		p.hasHitTheFloor = false;

		m_vecProjectiles[i] = p;
	}

	tempEnemy = Transform(glm::vec2(0.0f, 0.0f), glm::vec2(5.0f, 5.0f));


	m_pScript->AddWeapon(sprite);
}

Grenade::~Grenade()
{
}

void Grenade::OnAttach()
{
}

void Grenade::OnRender(float dt)
{
}

void Grenade::OnUnattach()
{
}

void Grenade::OnUpdate(float dt)
{
	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		for (int i = 0; i < m_baseProjectileCount + m_pScript->m_projectileCount; i++)
		{

			SpawnProjectile();

			if (m_currentCooldown <= 0)
			{
				m_isMax = false;
				float percentageReduction = (m_baseProjectileCooldown * m_pScript->m_projectileCooldown) / 100;
				m_currentCooldown = m_baseProjectileCooldown - percentageReduction;
			}
		}

		g_app->m_audioManager->m_system->playSound(m_audio->m_sound, m_audio->m_group, false, &m_audio->m_channel);
	}

	if (m_vecProjectiles.size() <= 0)
		return;

	for (int i = 0; i < m_vecProjectiles.size(); i++)
	{
		if (!m_vecProjectiles[i].isSpawned)
			continue;

		if (m_vecProjectiles[i].duration > 0)
		{
			if (!m_isMoving) continue;

			//TEMPORARY
			//will need to be replaced with a more dynamic system, such as moving the same direction as the player is facing, diagonal shooting etc

			if(!(glm::distance(m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position, m_vecProjectiles[i].originPos) >= m_vecProjectiles[i].distance))
			{
				float PercentageIncrease = (m_baseProjectileSpeed * m_pScript->m_projectileSpeed) / 100;
				m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += (m_vecProjectiles[i].direction.x * 1000) * (m_baseProjectileSpeed + PercentageIncrease) * dt;
				m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.y += (m_vecProjectiles[i].direction.y * 1000) * (m_baseProjectileSpeed + PercentageIncrease) * dt;

			}
			else
			{
				if (!m_vecProjectiles[i].hasHitTheFloor)
				{
					m_vecProjectiles[i].hasHitTheFloor = true;
					m_manager->GetComponent<AnimatedSprite>(m_vecProjectiles[i].name)->setAnimation("Exploding");
				}
			}
			//TEMPORARY	

			m_vecProjectiles[i].duration -= dt;
		}
		else
		{
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position = glm::vec2(1000.0f, 1000.0f);
			m_vecProjectiles[i].isSpawned = false;
			m_vecProjectiles[i].hasHitTheFloor = false;
			m_manager->GetComponent<AnimatedSprite>(m_vecProjectiles[i].name)->setAnimation("Throwing");
			i--;
		}
	}

	//COLLISION SHOULD BE TAKING PLACE IN CHECK COLLISION FUNCTION : )

	m_playerPreviousPosition = currentPosition;
}

void Grenade::SpawnProjectile()
{

	//std::vector<Entity> closeEnemies = Script::GetNearbyEntities();

	Projectiles* newProjectile = &m_vecProjectiles[m_currentProjectile];
	newProjectile->isSpawned = true;

	m_currentProjectile++;
	if (m_currentProjectile >= m_vecProjectiles.size() - 1)
		m_currentProjectile = 0;

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_manager->GetComponent<Transform>(newProjectile->name)->m_position = currentPosition;

	glm::vec2 direction(0, 0);


	direction = tempEnemy.m_position - currentPosition;

	if (direction != glm::vec2(0,0))
		direction = glm::normalize(direction);

	float PercentageIncrease = (m_baseProjectileArea * m_pScript->m_projectileDuration) / 100;

	newProjectile->duration = m_baseProjectileDuration + PercentageIncrease;
	newProjectile->direction = direction;
	newProjectile->originPos = currentPosition;
	newProjectile->distance = glm::distance(tempEnemy.m_position, currentPosition);

}