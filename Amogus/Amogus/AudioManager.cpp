#include "AudioManager.h"
#include <stdlib.h>

AudioManager::AudioManager() 
{	
    FMOD_RESULT result;
    AudioManagerInit();

    sfx = nullptr;
    result = m_system->createChannelGroup("sfx", &sfx);
    bgm = nullptr;
    result = m_system->createChannelGroup("bgm", &bgm);
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

//1 full volume, 0 silent
void AudioManager::SetVolume(FMOD::ChannelGroup* group, float volume)
{
    group->setVolume(volume);
}
