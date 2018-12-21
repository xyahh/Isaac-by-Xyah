#include "stdafx.h"
#include "Sound.h"
#include "Dependencies/FMOD/fmod.hpp"

Sound::Sound()
{
	FMOD::System_Create(&m_SoundSystem);
	m_SoundSystem->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);
}

Sound::~Sound()
{
	ReleaseSounds();
	m_SoundSystem->close();
	m_SoundSystem->release();
}

void Sound::Stop()
{
	FMOD::ChannelGroup* cg_master{ nullptr };
	m_SoundSystem->getMasterChannelGroup(&cg_master);
	cg_master->stop();
}

void Sound::Add(const STD string & SoundID, const STD string & path, bool isBGM)
{
	m_Sounds.emplace_back();
	size_t Index = Last(m_Sounds);
	m_SoundLocator[SoundID] = Index;

	if (isBGM) 
		m_SoundSystem->createStream(path.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &m_Sounds[Index]);
	else 
		m_SoundSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_Sounds[Index]);
}

void Sound::Update()
{
	for (size_t i = 0; i < m_SoundChannel.size(); ++i)
	{
		if (!IsPlaying(i))
		{
			EraseByValue(m_SoundChannel, m_SoundChannel[i]);
		}
	}
}

void Sound::Play(const STD string& ID)
{
	if (m_SoundChannel.size() == FMOD_MAX_CHANNEL_WIDTH) return;
	m_SoundChannel.emplace_back();
	m_SoundSystem->playSound(m_Sounds[m_SoundLocator[ID]], nullptr, false, &m_SoundChannel[Last(m_SoundChannel)]);
}

bool Sound::IsPlaying(size_t i)
{
	if (!m_SoundChannel[i])
		return false;
	bool bPlaying;
	m_SoundChannel[i]->isPlaying(&bPlaying);
	return bPlaying;
}

void Sound::ReleaseSounds()
{
	for (auto& s : m_Sounds)
		s->release();
}
