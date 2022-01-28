#pragma once
#include "fmod.hpp"

#include <string>

void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class Audio
{
public:
	Audio(const char* path, FMOD::System* system, FMOD::ChannelGroup* group);
	~Audio();

	void AddAudio(const char* path);
	void RemoveAudio();

	void AddToGroup(FMOD::ChannelGroup* group);

	void PauseAudio();
	void UnpauseAudio();

	void PlayAudio();
	void Loop();

	const FMOD::ChannelGroup* GetChannelGroup() const { return m_group; }

	bool IsPlaying = false;

	FMOD::Channel* m_channel;
	const std::string m_filePath;

private:
	FMOD::System* m_system;
	FMOD::ChannelGroup* m_group;
	FMOD::Sound* m_sound;
	FMOD_RESULT	m_result;
	void* m_extradriverdata = 0;
};

