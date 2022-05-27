#include "OptionsMenu.h"

OptionsMenu::OptionsMenu(EntityManager* entityManager, Entity parentEntityID) :
	Script(entityManager, parentEntityID)
{
	m_elements = GetComponent<UI_WidgetComponent>()->m_elements;

	m_bgmDown = (UI_ImageButton*)m_elements[3]; //3
	m_bgmUp = (UI_ImageButton*)m_elements[4]; //4
	m_sfxDown = (UI_ImageButton*)m_elements[5]; //5
	m_sfxUp = (UI_ImageButton*)m_elements[6]; //6
	m_close = (UI_ImageButton*)m_elements[7]; //7
	m_mute = (UI_ImageButton*)m_elements[8];//8

	m_testSFX = entityManager->AddComponent<Audio>(parentEntityID, "sfx/Weapons/shuriken.mp3", g_app->m_audioManager->m_system, g_app->m_audioManager->m_sfx);

}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::OnAttach()
{
}

void OptionsMenu::OnUpdate(float dt)
{

	if (m_bgmDown->m_state != ButtonState::BS_Click)
		bgmDownClick = false;
	if (m_sfxDown->m_state != ButtonState::BS_Click)
		sfxDownClick = false;
	if (m_bgmUp->m_state != ButtonState::BS_Click)
		bgmUpClick = false;
	if (m_sfxUp->m_state != ButtonState::BS_Click)
		sfxUpClick = false;
	if (m_close->m_state != ButtonState::BS_Click)
		closeClick = false;
	if (m_mute->m_state != ButtonState::BS_Click)
		muteClick = false;
	
	if (m_close->m_state == ButtonState::BS_Click && closeClick == false)
	{
		closeClick = true;
		CloseMenu();
	}

	if (m_mute->m_state == ButtonState::BS_Click && muteClick == false)
	{
		muteClick = true;
		Mute();
	}

	if (!m_IsMuted)
	{
		if (m_bgmDown->m_state == ButtonState::BS_Click && bgmDownClick == false)
		{
			bgmDownClick = true;
			VolumeDown(0);
		}

		if (m_sfxDown->m_state == ButtonState::BS_Click && sfxDownClick == false)
		{
			sfxDownClick = true;
			VolumeDown(1);
		}

		if (m_bgmUp->m_state == ButtonState::BS_Click && bgmUpClick == false)
		{
			bgmUpClick = true;
			VolumeUp(0);
		}

		if (m_sfxUp->m_state == ButtonState::BS_Click && sfxUpClick == false)
		{
			sfxUpClick = true;
			VolumeUp(1);
		}
	}
	
}

void OptionsMenu::OnRender(float dt)
{
}

void OptionsMenu::OnUnattach()
{
}

void OptionsMenu::CloseMenu()
{
	for (UI_BaseElement* elements : m_elements)
		elements->m_hidden = true;
}

void OptionsMenu::VolumeDown(int type)
{
	float vol;

	if (type == 0)
	{
		vol = g_app->m_audioManager->GetVolume(g_app->m_audioManager->m_bgm);
		if (vol > 0.0f)
		{
			vol -= 1.f;
		}

		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_bgm, vol);
	}

	if (type == 1)
	{
		vol = g_app->m_audioManager->GetVolume(g_app->m_audioManager->m_sfx);
		
		if (vol > 0.0f)
		{
			vol -= 1.f;
		}

		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_sfx, vol);
		g_app->m_audioManager->m_system->playSound(m_testSFX->m_sound, m_testSFX->m_group, false, &m_testSFX->m_channel);
	}
}

void OptionsMenu::VolumeUp(int type)
{
	float vol;

	if (type == 0)
	{
		vol = g_app->m_audioManager->GetVolume(g_app->m_audioManager->m_bgm);
		if (vol < 10.0)
		{
			vol += 1.f;
		}

		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_bgm, vol);
	}

	if (type == 1)
	{
		vol = g_app->m_audioManager->GetVolume(g_app->m_audioManager->m_sfx);

		if (vol < 10.0)
		{
			vol += 1.f;
		}

		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_sfx, vol);
		g_app->m_audioManager->m_system->playSound(m_testSFX->m_sound, m_testSFX->m_group, false, &m_testSFX->m_channel);
	}
}

void OptionsMenu::Mute()
{
	if (!m_IsMuted)
	{
		m_IsMuted = true;

		m_prevBgmVol = g_app->m_audioManager->GetVolume(g_app->m_audioManager->m_bgm);
		m_prevSfxVol = g_app->m_audioManager->GetVolume(g_app->m_audioManager->m_sfx);

		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_bgm, 0.f);
		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_sfx, 0.f);
	}
	else
	{
		m_IsMuted = false;
		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_bgm, m_prevBgmVol);
		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_sfx, m_prevSfxVol);

	}

}
