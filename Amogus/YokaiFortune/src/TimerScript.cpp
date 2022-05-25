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

	int minutes = ((int)m_seconds) / 60;
	int secondsToDisplay = ((int)m_seconds) % 60;

	std::string displayMinutes = std::to_string(minutes);
	if (minutes < 10)
	{
		displayMinutes = std::string("0") + displayMinutes;
	}

	std::string displaySeconds = std::to_string(secondsToDisplay);
	if (secondsToDisplay < 10)
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
