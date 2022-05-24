#include "QuitButtonScript.h"

QuitButtonScript::QuitButtonScript(EntityManager* entityManager, Entity parentEntityID) :
	Script(entityManager, parentEntityID)
{
	m_UIWidget = entityManager->GetComponent<UI_WidgetComponent>(parentEntityID);

	for (UI_BaseElement* element : m_UIWidget->m_elements)
	{
		if (element->m_elementType == ElementType::ET_ImageButton)
			button = (UI_ImageButton*)element;
	}
}

QuitButtonScript::~QuitButtonScript()
{

}

void QuitButtonScript::OnAttach()
{
}

void QuitButtonScript::OnUpdate(float dt)
{
	if (button->m_state == ButtonState::BS_Click)
	{
		GotoScene("StartScene");
	}
}

void QuitButtonScript::OnRender(float dt)
{
}

void QuitButtonScript::OnUnattach()
{
}
