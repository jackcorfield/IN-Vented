#pragma once
#include "fmod.hpp"

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void AddAudio();
	void ReleaseAudio(FMOD::Sound &sound);

};

