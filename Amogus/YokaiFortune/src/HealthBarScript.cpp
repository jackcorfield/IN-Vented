#include "HealthBarScript.h"

#include "PlayerScript.h"

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
	glm::vec2 healthBarPosition = playerTransform->m_position + glm::vec2(0.0f, 20.0f);

	// Set new size and position in uiWidget
	uiWidget->GetElement("HealthBarImage")->m_absolutePosition = healthBarPosition;
	uiWidget->GetElement("HealthBarImage")->m_absoluteSize = glm::vec2(percentageHealth * m_maxImageWidth, 1.0f);
}
void HealthBarScript::OnRender(float dt)
{}

void HealthBarScript::OnUnattach()
{}