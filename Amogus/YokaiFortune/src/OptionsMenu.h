#pragma once
#include "Resources\Script.h"
#include "Amogus.h"

class OptionsMenu :
    public Script
{
public:
	OptionsMenu(EntityManager* entityManager, Entity parentEntityID);
	~OptionsMenu();

	void OnAttach() override;
	void OnUpdate(float dt) override;
	void OnRender(float dt) override;
	void OnUnattach() override;

	void CloseMenu();
	void VolumeDown(int type);
	void VolumeUp(int type);
	void Mute();

	int m_prevSfxVol = 0;
	int m_prevBgmVol = 0;

	Audio* m_testSFX;

	bool bgmUpClick = false;
	bool bgmDownClick = false;
	bool sfxUpClick = false;
	bool sfxDownClick = false;
	bool muteClick = false;
	bool closeClick = false;

	bool m_IsMuted = false;
	std::vector<UI_BaseElement*> m_elements;
	//UI_WidgetComponent* m_UIWidget;

	UI_ImageButton* m_bgmDown; //3
	UI_ImageButton* m_bgmUp; //4
	UI_ImageButton* m_sfxDown; //5
	UI_ImageButton* m_sfxUp; //6
	UI_ImageButton* m_close; //7
	UI_ImageButton* m_mute;//8

};

