#pragma once
#include "fmod.hpp"
class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool AudioInit();

	void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
	#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

	FMOD::System*		m_system;
	FMOD::Sound*			m_sound;
	FMOD::Channel*		m_channel = 0;
	FMOD_RESULT       m_result;
	void*						m_extradriverdata = 0;

};

