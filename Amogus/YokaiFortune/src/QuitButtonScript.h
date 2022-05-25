#pragma once
#include "Resources\Script.h"
#include "Amogus.h"

class QuitButtonScript :
	public Script
{
public:
	QuitButtonScript(EntityManager* entityManager, Entity parentEntityID);
	~QuitButtonScript();

	void OnAttach() override;
	void OnUpdate(float dt) override;
	void OnRender(float dt) override;
	void OnUnattach() override;

	UI_WidgetComponent* m_UIWidget;
	UI_ImageButton* button;

	bool loadMainScene = false;
};


