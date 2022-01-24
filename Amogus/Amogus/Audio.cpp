#include "Audio.h"
#include <stdio.h>
#include <iostream>
#include "fmod_common.h"

Audio::Audio(const char* path, FMOD::System* system ,FMOD::ChannelGroup* group)
{
    m_system = system;
    m_group = group;
    m_channel = 0;
    AddAudio(path);
    AddToGroup(group);
}


void Audio::AddAudio(const char* path)
{
    m_system->createSound(path, FMOD_DEFAULT, 0, &m_sound);
}

void Audio::AddToGroup(FMOD::ChannelGroup* group)
{
    m_channel->setChannelGroup(m_group);
}

void Audio::RemoveAudio()
{
    m_system->release();
    IsPlaying = false;
}

void Audio::PauseAudio()
{
    m_channel->setPaused(true);
}

void Audio::UnpauseAudio()
{
    m_channel->setPaused(false);
}

void Audio::PlayAudio()
{
    m_system->playSound(m_sound, m_group, false, &m_channel);
    IsPlaying = true;
}