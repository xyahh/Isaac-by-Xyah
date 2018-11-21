#include "stdafx.h"
#include "Sound.h"
#include "Dependencies/FMOD/fmod.hpp"

bool				Sound::m_isPaused		{ true };
unsigned long long	Sound::m_ParentClock	{ 0 };
FMOD::DSP*			Sound::m_DSP			{ NULL };
FMOD::System*		Sound::m_SoundSystem	{ NULL };

Sound::Sound(const STD string& path, bool isBGM)
{
	FMOD_RESULT result;
	m_Sound = nullptr;
	m_IsBGM = isBGM;

	if (m_IsBGM)
		result = m_SoundSystem->createStream(path.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &m_Sound);
	else
		result = m_SoundSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_Sound);
}

void Sound::Initialize()
{
	FMOD::System_Create(&m_SoundSystem);
	m_SoundSystem->init(FMOD_MAX_CHANNEL_WIDTH,
		FMOD_INIT_NORMAL, nullptr);
}

void Sound::Destroy()
{
	m_SoundSystem->close();
	m_SoundSystem->release();
}

void Sound::Release()
{
	m_Sound->release();
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
	if (m_IsBGM)
		m_SoundSystem->playSound(m_Sound, nullptr, false, &m_SoundChannel);
	else
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