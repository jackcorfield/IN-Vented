#pragma once
#include "fmod.hpp"

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void AddAudio();
	bool AudioManagerInit();

	void SetVolume(FMOD::ChannelGroup* group, float volume);

	FMOD::ChannelGroup*	sfx;
	FMOD::ChannelGroup*	bgm;
	FMOD::ChannelControl*	master;

	FMOD::System* m_system;
};

