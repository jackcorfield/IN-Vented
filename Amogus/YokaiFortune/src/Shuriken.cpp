#include "Shuriken.h"

Shuriken::Shuriken(EntityManager* entityManager, Entity parentEntityID, Entity player, Sprite icon, Sprite sprite, glm::vec2 hitboxSize, int level, bool moving, bool autoTarget) : 
	WeaponScript(entityManager, parentEntityID, player, icon, sprite, hitboxSize, level, moving, autoTarget)
{
	m_baseProjectileSpeed = 10; //Speed of projectiles
	m_baseProjectileCooldown = 1; //How often weapon attacks
	m_baseProjectileArea = 1; //Size of weapon
	m_baseProjectileDuration = 4; //How long the projectile stays on the screen
	m_baseProjectileCount = 3; //How many projectiles
	m_projectileMax = 6;

	m_baseDamageModifier = 1;

	m_hitDelay = 1;
	m_critMultiplier = 1;

	m_wait = 0.1;

	m_playerPreviousPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	std::srand(time(NULL));
}

Shuriken::~Shuriken()
{
}

void Shuriken::OnAttach()
{
}

void Shuriken::OnRender(float dt)
{
}

void Shuriken::OnUnattach()
{
}

void Shuriken::OnUpdate(float dt)
{
	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_currentCooldown -= dt;

	if (m_currentCooldown <= 0)
	{
		for (int i = 0; i < m_baseProjectileCount; i++)
		{

			SpawnProjectile();

			if (m_currentCooldown <= 0)
			{
				m_isMax = false;
				m_currentCooldown = m_baseProjectileCooldown;
			}
		}
	}

	if (m_vecProjectiles.size() <= 0)
		return;

	for (int i = 0; i < m_vecProjectiles.size(); i++)
	{
		if (m_vecProjectiles[i].duration > 0)
		{
			if (!m_isMoving)
			{
				continue;
			}

			//TEMPORARY
			//will need to be replaced with a more dynamic system, such as moving the same direction as the player is facing, diagonal shooting etc


			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += (m_vecProjectiles[i].direction.x * 1000) * dt;
			m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.y += (m_vecProjectiles[i].direction.y * 1000) * dt;
			//m_manager->GetComponent<Transform>(m_vecProjectiles[i].name)->m_position.x += 100 * dt;

			//TEMPORARY	

			m_vecProjectiles[i].duration -= dt;
		}
		else
		{
			Entity entity = m_vecProjectiles[i].name;
			m_vecProjectiles.erase(m_vecProjectiles.begin() + i);
			m_manager->DeleteEntity(entity);
			i--;
		}
	}

	//COLLISION SHOULD BE TAKING PLACE IN CHECK COLLISION FUNCTION : )

	m_playerPreviousPosition = currentPosition;
}

void Shuriken::SpawnProjectile()
{

	glm::vec2 offset;

	offset.x = (rand() % 200 - 100) / 2;
	offset.y = (rand() % 200 - 100) / 2;

	//MAKE BASE SET
	//IF MORE ARE NEEDED (EG THE CURRENT ARE STILL IN USE)
	//ADD MORE ENTITIES TO THE LIST
	//IF NOT, REUSE OLD ENTITIES

	Entity newProjectile = m_manager->CreateEntity();

	Transform* transform = GetComponent<Transform>();

	glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

	m_manager->AddComponent<Transform>(newProjectile, glm::vec2(0, 0), glm::vec2(.25f * m_baseProjectileArea, .25f * m_baseProjectileArea));

	m_manager->GetComponent<Transform>(newProjectile)->m_position = currentPosition + offset;

	m_manager->AddComponent<Sprite>(newProjectile, m_sprite.GetTexture(), m_sprite.GetColour(), m_sprite.GetShader()); //replace later with animated sprite!
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

	m_vecProjectiles.push_back(p);

	//play noise
}