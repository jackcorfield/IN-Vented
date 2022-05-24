#include "XpManagerScript.h"

XpManager::XpManager(EntityManager* entityManager, Entity parentEntityID, Entity player, int maxOrbs, int poolingSize) :
	Script(entityManager, parentEntityID),
	m_player(player),
	m_maxOrbs(maxOrbs),
	m_poolingSize(poolingSize),
	m_manager(entityManager)
{
	AnimatedSprite* templateASprite = GetComponent<AnimatedSprite>();

	m_playerScript = (PlayerScript*)entityManager->GetComponent<ScriptComponent>(player)->GetAttachedScript();

	m_orbs.resize(m_maxOrbs);

	for (int i = 0; i < m_maxOrbs; i++)
	{
		Entity newProjectile = m_manager->CreateEntity();

		Transform* transform = GetComponent<Transform>();
		glm::vec2 currentPosition = m_manager->GetComponent<Transform>(m_player)->m_position;

		m_manager->AddComponent<Transform>(newProjectile, glm::vec2(100000.0f, 100000.0f), glm::vec2(.1,.1), 0.1f);
		m_manager->AddComponent<BoxCollider>(newProjectile, glm::vec2(30), glm::vec2(0.0f));
		m_manager->AddComponent<EntityName>(newProjectile, "XpOrb");


		AnimatedSprite* newASprite = m_manager->AddComponent<AnimatedSprite>(newProjectile, templateASprite->GetTexture(), templateASprite->getFrameSize(), templateASprite->GetColour(), templateASprite->GetShader());
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
		newASprite->setAnimation("Small");

		Orb o;

		o.id = newProjectile;
		o.isSpawned = false;
		o.xpVal = 100;

		m_orbs[i] = o;

	}

}

void XpManager::OnAttach()
{

}

void XpManager::OnUpdate(float dt)
{
	std::vector<Entity> collisions = g_app->m_collisionManager->potentialCollisions(m_player);

	for (Entity e: collisions)
	{
		
		EntityName* name = m_manager->GetComponent<EntityName>(e);
		
		if (name && name->m_name == "XpOrb")
		{
			if (g_app->m_collisionManager->checkCollision(m_player, e))
			{
				
				Orb* collidedOrb = FindOrb(e);
				collidedOrb->isSpawned = false;

				m_playerScript->AddXP(collidedOrb->xpVal);

				m_manager->GetComponent<Transform>(e)->m_position = glm::vec2(100000.0f, 100000.0f);
			}
		}

	}
}

void XpManager::OnRender(float dt)
{
	
}

void XpManager::OnUnattach()
{

}

XpManager::Orb *XpManager::FindOrb(Entity OrbID)
{

	for (Orb& orb : m_orbs)
	{
		if (orb.id == OrbID)
			return &orb;
	}

	return nullptr;
}

void XpManager::SpawnOrb(glm::vec2 position, int XpVal, bool isRecurse)
{
	bool poolFull = true;
	for (Orb &orb : m_orbs)
	{
		if (!orb.isSpawned)
		{
			orb.isSpawned = true;
			orb.xpVal = XpVal;
			if (XpVal > 500)
				m_manager->GetComponent<AnimatedSprite>(orb.id)->setAnimation("Large");
			else
				m_manager->GetComponent<AnimatedSprite>(orb.id)->setAnimation("Small");

			m_manager->GetComponent<Transform>(orb.id)->m_position = position;

			poolFull = false;

			break;
		}
	}

	if (poolFull && !isRecurse)
	{
		//Clean Pool
		int pooledXP = 0;
		int noPooledOrbs = 0;
		glm::vec2 newOrbPos;

		for (Orb& orb : m_orbs)
		{
				
			if (orb.isSpawned)
			{
				pooledXP += orb.xpVal;
				noPooledOrbs++;

				orb.isSpawned = false;

				newOrbPos = m_manager->GetComponent<Transform>(orb.id)->m_position;
				
				m_manager->GetComponent<Transform>(orb.id)->m_position = glm::vec2(1000000.0f);
			}

			if (noPooledOrbs >= m_poolingSize)
				break;

		}

		SpawnOrb(newOrbPos, pooledXP);
	}
}