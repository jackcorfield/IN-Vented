#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class StartMenuButton :
	public Script
{
public:
	StartMenuButton(EntityManager* entityManager, Entity parentEntityID);
	~StartMenuButton();

	void OnAttach() override;
	void OnUpdate(float dt) override;
	void OnRender(float dt) override;
	void OnUnattach() override;

	UI_WidgetComponent* m_UIWidget;
	UI_ImageButton* button;
};

