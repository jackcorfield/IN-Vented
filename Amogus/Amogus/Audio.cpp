#include "Audio.h"
#include <stdio.h>
#include <iostream>
#include "fmod_common.h"

Audio::Audio(const char* path, FMOD::System* system, FMOD::ChannelGroup* group):
  m_filePath(path),
	m_system(system),
	m_group(group)
{
	m_system->createSound(path, FMOD_DEFAULT, 0, &m_sound);
	//m_system->playSound(m_sound, group, false, &m_channel);
}