#include "StartMenuButton.h"

StartMenuButton::StartMenuButton(EntityManager* entityManager, Entity parentEntityID) :
	Script(entityManager, parentEntityID)
{
	m_UIWidget = entityManager->GetComponent<UI_WidgetComponent>(parentEntityID);

	for (UI_BaseElement* element : m_UIWidget->m_elements)
	{
		if (element->m_elementType == ElementType::ET_ImageButton)
			button = (UI_ImageButton*)element;
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
	if (button->m_state == ButtonState::BS_Click)
	{
		loadMainScene = true;
	}
}

void StartMenuButton::OnRender(float dt)
{
}

void StartMenuButton::OnUnattach()
{
}
