#include "AudioManager.h"
#include "fmod.h"
#include <stdio.h>
#include <iostream>

AudioManager::AudioManager() 
{	

}

bool AudioManager::AudioInit()
{
        FMOD_RESULT result;
        result = FMOD::System_Create(&m_system);
        if (result != FMOD_OK)
        {
            return false;
        }
        result = m_system->init(50, FMOD_INIT_NORMAL, NULL);
        if (result != FMOD_OK)
        {
            return false;
        }

        m_system->createSound("Assets/Audio/Diesel.wav", FMOD_DEFAULT, 0, &m_sound);
        m_system->playSound(m_sound, NULL, false, &m_channel);

        return true;
}