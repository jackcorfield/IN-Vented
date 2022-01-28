#pragma once
#include "fmod.hpp"

void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class Audio
{
public:
	Audio(const char* path, FMOD::System* system, FMOD::ChannelGroup* group);
	~Audio();

	FMOD::Channel* m_channel =0;
	FMOD::Sound* m_sound;
	FMOD::System* m_system;
	FMOD::ChannelGroup* m_group;
	
};

