#include "Audio.h"
#include <stdio.h>
#include <iostream>
#include "fmod_common.h"

Audio::Audio(const char* path, FMOD::System* system, FMOD::ChannelGroup* group) :
	m_filePath(path),
	m_system(system),
	m_group(group)
{
	std::string p = "Assets/Audio/" + std::string(path);
	m_system->createSound(p.c_str(), FMOD_DEFAULT, 0, &m_sound);
}