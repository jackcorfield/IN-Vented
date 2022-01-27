#pragma once
#include "fmod.hpp"

void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class Audio
{
public:
	Audio(const char* path, FMOD::System* system, FMOD::ChannelGroup* group);
	~Audio();

	bool AudioInit();

	void AddAudio(const char* path);
	void RemoveAudio();

	void AddToGroup(FMOD::ChannelGroup* group);

	void PauseAudio();
	void UnpauseAudio();

	void PlayAudio();
	void Loop();

	bool IsPlaying = false;

	FMOD::Channel* m_channel;
private:

	FMOD::System*		m_system;
	FMOD::ChannelGroup* m_group;
	FMOD::Sound*			m_sound;
	FMOD_RESULT		m_result;
	void*						m_extradriverdata = 0;
};

