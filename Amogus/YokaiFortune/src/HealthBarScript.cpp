#include "HealthBarScript.h"

#include "PlayerScript.h"

#define GAME_VIEWPORT_HEIGHT 180.0f
#define Y_OFFSET 0.05f

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

	// Calculate health bar size and position
	float currentHealth = playerScript->m_health;
	float maxHealth = playerScript->m_maxHealth;
	float percentageHealth = currentHealth / maxHealth;

	float min = -GAME_VIEWPORT_HEIGHT;
	float max = GAME_VIEWPORT_HEIGHT;
	float percentageY = (playerTransform->m_position.y - min) / (max - min) / 2.0f;
	float relativeYPos = percentageY + Y_OFFSET;

	glm::vec2 healthBarPosition = glm::vec2(0.25f, relativeYPos);
	glm::vec2 prevSize = uiWidget->GetElement("HealthBarImage")->m_absoluteSize;

	// Set new size and position in uiWidget
	uiWidget->GetElement("HealthBarImage")->m_relativePosition = healthBarPosition;
	uiWidget->GetElement("HealthBarImage")->m_absoluteSize = glm::vec2(percentageHealth * m_maxImageWidth, prevSize.y);
}
void HealthBarScript::OnRender(float dt)
{}

void HealthBarScript::OnUnattach()
{}