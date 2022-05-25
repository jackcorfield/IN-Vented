#include "StartMenuButton.h"

StartMenuButton::StartMenuButton(EntityManager* entityManager, Entity parentEntityID, Entity optionsMenu) :
	Script(entityManager, parentEntityID)
{
	m_UIWidget = entityManager->GetComponent<UI_WidgetComponent>(parentEntityID);
	m_optionsMenu = entityManager->GetComponent<UI_WidgetComponent>(optionsMenu);

	for (UI_BaseElement* element : m_UIWidget->m_elements)
	{

		if (!m_startButton)
		{
			if (element->m_elementType == ElementType::ET_ImageButton)
				m_startButton = (UI_ImageButton*)element;
		}
		else if(!m_quitButton)
		{
			if (element->m_elementType == ElementType::ET_ImageButton)
				m_quitButton = (UI_ImageButton*)element;
		}
		else if (!m_optionsButton)
		{
			if (element->m_elementType == ElementType::ET_ImageButton)
				m_optionsButton = (UI_ImageButton*)element;
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

	if (m_optionsButton->m_state == ButtonState::BS_Click)
	{
		for (UI_BaseElement* element : m_optionsMenu->m_elements)
			element->m_hidden = false;
	}

}

void StartMenuButton::OnRender(float dt)
{
}

void StartMenuButton::OnUnattach()
{
}
