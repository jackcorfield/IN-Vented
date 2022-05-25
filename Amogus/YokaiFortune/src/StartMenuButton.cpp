#include "StartMenuButton.h"

StartMenuButton::StartMenuButton(EntityManager* entityManager, Entity parentEntityID) :
	Script(entityManager, parentEntityID)
{
	m_UIWidget = entityManager->GetComponent<UI_WidgetComponent>(parentEntityID);

	for (UI_BaseElement* element : m_UIWidget->m_elements)
	{

		if (!m_startButton)
		{
			if (element->m_elementType == ElementType::ET_ImageButton)
				m_startButton = (UI_ImageButton*)element;
		}
		else
		{
			if (element->m_elementType == ElementType::ET_ImageButton)
				m_quitButton = (UI_ImageButton*)element;
		}
	}

}

StartMenuButton::~StartMenuButton()
{
}

void StartMenuButton::OnAttach()
{
}

void StartMenuButton::OnUpdate(float dt)
{
	if (m_startButton->m_state == ButtonState::BS_Click)
	{
		loadMainScene = true;
	}

	if (m_quitButton->m_state == ButtonState::BS_Click)
	{
		g_app->Quit();
	}
}

void StartMenuButton::OnRender(float dt)
{
}

void StartMenuButton::OnUnattach()
{
}
