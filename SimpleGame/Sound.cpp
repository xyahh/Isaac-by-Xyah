#include "stdafx.h"
#include "Sound.h"
#include "Dependencies/FMOD/fmod.hpp"


Sound::Sound(const STD string& path, bool isBGM)
{
	
	FMOD::System_Create(&m_SoundSystem);
	m_SoundSystem->init(1, FMOD_INIT_NORMAL, nullptr);

	m_isPaused = true;
	m_ParentClock = 0;
	m_DSP = NULL;

	FMOD_RESULT result;
	m_Sound = nullptr;

	if (isBGM)
		result = m_SoundSystem->createStream(path.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &m_Sound);
	else
		result = m_SoundSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_Sound);
}

void Sound::Release()
{
	m_Sound->release();
	m_SoundSystem->close();
	m_SoundSystem->release();
}

void Sound::Stop(bool master_stop)
{
	if (!m_SoundChannel) return;

	m_SoundChannel->stop();
	if (!master_stop) return;
	FMOD::ChannelGroup* cg_master{ nullptr };
	m_SoundSystem->getMasterChannelGroup(&cg_master);
	cg_master->stop();
}

void Sound::Play()
{
	m_SoundSystem->playSound(m_Sound, nullptr, false, &m_SoundChannel);
	m_isPaused = false;
}

void Sound::SetPause(bool state)
{
	if (!m_SoundChannel) return;
	m_SoundChannel->setPaused(state);
	m_isPaused = state;
}

void Sound::FadeOut(int samples)
{
	if (!m_SoundChannel) return;

	m_SoundChannel->getDSPClock(NULL, &m_ParentClock);
	m_SoundChannel->addFadePoint(m_ParentClock, 1);
	m_SoundChannel->addFadePoint(m_ParentClock + samples, 0);
	m_SoundChannel->setDelay(0, m_ParentClock + samples, true);
}

bool Sound::IsPlaying()
{
	if (!m_SoundChannel) 
		return false;

	bool bPlaying;
	m_SoundChannel->isPlaying(&bPlaying);
	return bPlaying && !m_isPaused;
}