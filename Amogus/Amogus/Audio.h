#pragma once
#include "fmod.hpp"

class Audio
{
public:
	Audio();
	Audio(const char* path);
	~Audio();

	bool AudioInit();

	void AddAudio(const char* path);
	void RemoveAudio();

	void PauseAudio();
	void PlayAudio();

	bool IsPlaying = false;

	void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
	#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

private:

	FMOD::System* m_system;
	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel = 0;
	FMOD_RESULT       m_result;
	void* m_extradriverdata = 0;
};

