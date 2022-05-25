#include "XpBarScript.h"

#include "PlayerScript.h"

XpBarScript::XpBarScript(EntityManager* entityManager, Entity parentEntityID, Entity playerEntityID) :
	Script(entityManager, parentEntityID),
	m_trackedPlayer(playerEntityID)
{}

void XpBarScript::OnAttach()
{}

void XpBarScript::OnUpdate(float dt)
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	// Get relevant components
	UI_WidgetComponent* uiWidget = GetComponent<UI_WidgetComponent>();
	Transform* playerTransform = entityManager->GetComponent<Transform>(m_trackedPlayer);
	ScriptComponent* playerScriptComponent = entityManager->GetComponent<ScriptComponent>(m_trackedPlayer);
	PlayerScript* playerScript = reinterpret_cast<PlayerScript*>(playerScriptComponent->GetAttachedScript());

	// XP progress bar
	// Size
	float currentXP = playerScript->GetXP();
	float maxXP = playerScript->GetXPToLevelUp();
	float percentageXP = currentXP / maxXP;

	glm::vec2 prevSize = uiWidget->GetElement("XpCurrent")->m_absoluteSize;

	// Position
	glm::vec2 position = glm::vec2(0.5f, 0.95f);

	// Colour
	glm::vec3 colour(0.1f, 0.1f, 1.0f);

	// Set new size, position and colour in uiWidget
	UI_BaseElement* sprite = uiWidget->GetElement("XpCurrent");
	sprite->m_relativePosition = position;
	sprite->m_absoluteSize = glm::vec2(percentageXP * m_maxImageWidth, prevSize.y);
	sprite->m_colour = colour;

	// XP background bar
	// Size
	prevSize = uiWidget->GetElement("XpBackground")->m_absoluteSize;

	// Colour
	colour = glm::vec3(0.1f, 0.1f, 0.5f);

	// Set new size, position and colour in uiWidget
	sprite = uiWidget->GetElement("XpBackground");

	sprite->m_relativePosition = position;
	sprite->m_absoluteSize = glm::vec2(m_maxImageWidth, prevSize.y);
	sprite->m_colour = colour;
}