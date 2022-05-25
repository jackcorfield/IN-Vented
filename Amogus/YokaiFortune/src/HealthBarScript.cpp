#include "HealthBarScript.h"

#include "PlayerScript.h"

#define GAME_VIEWPORT_HEIGHT 180.0f
#define Y_OFFSET 0.08f

HealthBarScript::HealthBarScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID) :
	Script(entityManager, parentEntityID),
	m_trackedPlayer(playerEntityID)
{}

void HealthBarScript::OnAttach()
{}

void HealthBarScript::OnUpdate(float dt)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Get relevant components
	UI_WidgetComponent* uiWidget = GetComponent<UI_WidgetComponent>();
	Transform* playerTransform = entityManager->GetComponent<Transform>(m_trackedPlayer);
	ScriptComponent* playerScriptComponent = entityManager->GetComponent<ScriptComponent>(m_trackedPlayer);
	PlayerScript* playerScript = reinterpret_cast<PlayerScript*>(playerScriptComponent->GetAttachedScript());

	// Calculate size
	float currentHealth = playerScript->m_health;
	float maxHealth = playerScript->m_maxHealth;
	float percentageHealth = currentHealth / maxHealth;

	glm::vec2 prevSize = uiWidget->GetElement("HealthBarImage")->m_absoluteSize;

	// Calculate position
	float min = -GAME_VIEWPORT_HEIGHT;
	float max = GAME_VIEWPORT_HEIGHT;
	float percentageY = (playerTransform->m_position.y - min) / (max - min);
	float relativeYPos = percentageY + Y_OFFSET;

	glm::vec2 healthBarPosition = glm::vec2(0.5f, relativeYPos);

	// Calculate colour
	float red = 1.0f + percentageHealth * (0.0f - 1.0f); // Higher when health percentage is lower
	float green = 0.0f + percentageHealth * (1.0f - 0.0f); // Higher when health percentage is higher
	glm::vec3 healthBarColour(red, green, 0.0f);
	
	UI_BaseElement* sprite = uiWidget->GetElement("HealthBarImage");

	// Set new size, position and colour in uiWidget
	sprite->m_relativePosition = healthBarPosition;
	sprite->m_absoluteSize = glm::vec2(percentageHealth * m_maxImageWidth, prevSize.y);
	sprite->m_colour = healthBarColour;
}

void HealthBarScript::OnRender(float dt)
{}

void HealthBarScript::OnUnattach()
{}