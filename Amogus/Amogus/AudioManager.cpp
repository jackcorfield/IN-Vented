#include "AudioManager.h"
#include <stdlib.h>

AudioManager::AudioManager() 
{	
    FMOD_RESULT result;
    AudioManagerInit();

    m_sfx = nullptr;
    result = m_system->createChannelGroup("sfx", &m_sfx);
    m_bgm = nullptr;
    result = m_system->createChannelGroup("bgm", &m_bgm);
}

bool AudioManager::AudioManagerInit()
{
    FMOD_RESULT result;
    result = FMOD::System_Create(&m_system);
    if (result != FMOD_OK)
    {
        return false;
    }
    result = m_system->init(32, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
    {
        return false;
    }

    return true;
}