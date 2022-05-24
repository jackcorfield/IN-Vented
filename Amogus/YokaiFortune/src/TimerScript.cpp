#include "TimerScript.h"

TimerScript::TimerScript(EntityManager* entityManager, Entity parentEntityID, Entity Timer) :
	Script(entityManager, parentEntityID)
{

}

TimerScript::~TimerScript()
{
}

void TimerScript::OnAttach()
{
}

void TimerScript::OnUpdate(float dt)
{
	m_seconds += dt;

	if (m_seconds > 59)
	{
		m_minutes += 1;
		m_seconds = 0;
	}

	UI_Text* temp = (UI_Text*)GetComponent<UI_WidgetComponent>()->m_elements[0];

	temp->m_text = std::to_string((int)m_minutes) + ":" + std::to_string((int)m_seconds);
}

void TimerScript::OnRender(float dt)
{
}

void TimerScript::OnUnattach()
{
}
