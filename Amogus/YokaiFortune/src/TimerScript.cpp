#include "TimerScript.h"

void TimerScript::OnAttach()
{
}

void TimerScript::OnUpdate(float dt)
{
	m_seconds += dt;

	if (m_seconds >= 59)
	{
		m_minutes += 1;
		m_seconds = 0;
	}

	//(UI_Text*)GetComponent<UI_WidgetComponent>()->m_elements[0]->;
}

void TimerScript::OnRender(float dt)
{
}

void TimerScript::OnUnattach()
{
}
