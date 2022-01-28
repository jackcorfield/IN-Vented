#pragma once
#include "EntityManager.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Camera.h"

enum class ComponentType
{
	eSprite = 0,
	eAnimatedSprite,
	eCamera,
	eUnknown
};

namespace ImGuiHelper
{
	ComponentType GetEntityType(Entity entity, EntityManager* manager)
	{
		{
			AnimatedSprite* animatedSprite = manager->GetComponent<AnimatedSprite>(entity);

			if (animatedSprite)
				return ComponentType::eAnimatedSprite;
		}

		{
			Sprite* sprite = manager->GetComponent<Sprite>(entity);
			
			if (sprite)
				return ComponentType::eSprite;
		}

		{
			Camera* camera = manager->GetComponent<Camera>(entity);

			if (camera)
				return ComponentType::eCamera;
		}

		return ComponentType::eUnknown;
	}
}