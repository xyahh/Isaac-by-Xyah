#pragma once

class Sound
{
	friend Cyan;

public:

	Sound(const STD string& path, bool isBGM);
	~Sound() { }

private:
	//Only Game Engine can access these functions
	static void Initialize();
	static void Destroy();
	void Release();

public:

	void Play();
	void Stop(bool master_stop);
	void FadeOut(int samples);
	void SetPause(bool state);
	bool IsPlaying();

private:

	bool						m_IsBGM;
	FMOD::Sound*				m_Sound;
	FMOD::Channel*				m_SoundChannel;

	static bool					m_isPaused;
	static unsigned long long	m_ParentClock;
	static FMOD::DSP*			m_DSP;
	static FMOD::System*		m_SoundSystem;
};