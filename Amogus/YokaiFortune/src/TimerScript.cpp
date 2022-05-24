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
	UI_Text* text = (UI_Text*)GetComponent<UI_WidgetComponent>()->m_elements[0];

	m_minutes = (int)m_seconds / 60;
	std::string displayMinutes = std::to_string((int)m_minutes);
	if (m_minutes < 10)
	{
		displayMinutes = std::string("0") + displayMinutes;
	}

	std::string displaySeconds = std::to_string((int)m_seconds);
	if (m_seconds < 10)
	{
		displaySeconds = std::string("0") + displaySeconds;
	}

	text->m_text = displayMinutes + ":" + displaySeconds;
}

void TimerScript::OnRender(float dt)
{
}

void TimerScript::OnUnattach()
{
}
