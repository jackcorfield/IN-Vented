#pragma once
#include "fmod.hpp"

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool						AudioManagerInit();

	void						PauseAudio(FMOD::Channel* channel) {channel->setPaused(true); };
	void						UnpauseAudio(FMOD::Channel* channel) { channel->setPaused(false); };

	void						SetAudio(const char* path, FMOD::Sound* sound) { m_system->createSound(path, FMOD_DEFAULT, 0, &sound); }
	void						PlayAudio(FMOD::Sound* sound, FMOD::ChannelGroup* group, FMOD::Channel* channel) { m_system->playSound(sound, group, false, &channel);  }
	void					    StopAudio(FMOD::Channel* channel) { channel->stop(); }
	void						RemoveAudio(FMOD::Sound* sound) { sound->release(); }

	void						SetGroup(FMOD::Channel* channel, FMOD::ChannelGroup* group) { channel->setChannelGroup(group); }
	void						SetLoopCount(FMOD::Sound* sound, int LoopCount) { sound->setLoopCount(LoopCount); }
	//1 full volume, 0 silent, going over 1 will make it louder but may distort audio
	void						SetVolume(FMOD::ChannelGroup* group, float volume) { group->setVolume(volume); }
	void						SetPosition(const float x, const float y, const float velocityX, const float velocityY, FMOD::Channel* channel) { FMOD_VECTOR pos = { x, y, 0.0f }; FMOD_VECTOR vel = { velocityX,velocityY, 0.0f }; channel->set3DAttributes(&pos, &vel); }

	FMOD::ChannelGroup*		GetGroup(FMOD::Channel* channel) { FMOD::ChannelGroup* group;  channel->getChannelGroup(&group); return group; }
	int									GetLoopCount(FMOD::Sound* sound) { int count;  sound->getLoopCount(&count); return count; }
	float									GetVolume(FMOD::ChannelGroup* group) { float volume;  group->getVolume(&volume); return volume; }

	//must be called BEFORE play Audio
	void									LoopOn(FMOD::Sound* sound)	{ sound->setMode(FMOD_LOOP_NORMAL); }
	void									LoopOff(FMOD::Sound* sound) { sound->setMode(FMOD_LOOP_OFF); }

	

	//std::vector<FMOD::Channel*>		m_channel; 
	FMOD::ChannelGroup*				m_sfx;
	FMOD::ChannelGroup*				m_bgm;
	FMOD::System*						m_system;
};

