#pragma once
#include "Resources/Script.h"
#include "Amogus.h"

class StartMenuButton :
	public Script
{
public:
	StartMenuButton(EntityManager* entityManager, Entity parentEntityID, Entity optionsMenu);
	~StartMenuButton();

	void OnAttach() override;
	void OnUpdate(float dt) override;
	void OnRender(float dt) override;
	void OnUnattach() override;

	UI_WidgetComponent* m_UIWidget;
	UI_WidgetComponent* m_optionsMenu;

	UI_ImageButton* m_startButton;
	UI_ImageButton* m_quitButton;
	UI_ImageButton* m_optionsButton;

	bool loadMainScene = false;
};

