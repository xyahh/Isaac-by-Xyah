#pragma once

class Sound
{
	friend Cyan;
public:

	Sound(const STD string& path, bool isBGM);
	~Sound() { }

private:

	void Release();

public:

	void Play();
	void Stop(bool master_stop);
	void FadeOut(int samples);
	void SetPause(bool state);
	bool IsPlaying();

private:
	FMOD::Sound*				m_Sound;
	FMOD::Channel*				m_SoundChannel;

	bool					m_isPaused;
	unsigned long long	m_ParentClock;
	FMOD::DSP*			m_DSP;
	FMOD::System*		m_SoundSystem;
};