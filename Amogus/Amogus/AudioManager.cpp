#include "AudioManager.h"


AudioManager::AudioManager() 
{	

}

void AudioManager::ReleaseAudio(FMOD::Sound& sound)
{
	sound.release();
}
