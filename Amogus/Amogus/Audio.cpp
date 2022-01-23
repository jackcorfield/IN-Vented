#include "Audio.h"
#include <stdio.h>
#include <iostream>

Audio::Audio()
{
    AudioInit();
}

Audio::Audio(const char* path)
{
    AudioInit();
    m_system->createSound(path, FMOD_DEFAULT, 0, &m_sound);
}

bool Audio::AudioInit()
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
    
    return true;
}

void Audio::AddAudio(const char* path)
{
    m_system->createSound(path, FMOD_DEFAULT, 0, &m_sound);
}

void Audio::RemoveAudio()
{
    m_system->release();
    IsPlaying = false;
}

void Audio::PauseAudio()
{
    //something to do with event instances??
}

void Audio::PlayAudio()
{
    m_system->playSound(m_sound, NULL, false, &m_channel);
    IsPlaying = true;
}