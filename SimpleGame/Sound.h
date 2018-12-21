#pragma once

class Sound : public EventDispatcher
{
	friend Cyan;
public:

	Sound();
	~Sound();

private:

	bool IsPlaying(size_t i);
	void ReleaseSounds();


public:

	void Add(const STD string& SoundID, const STD string& path, bool isBGM);

	void Update();

	void Play(const STD string& ID = "");
	void Stop();
	

private:

	FMOD::System*	m_SoundSystem;

	STD map<STD string, size_t>		m_SoundLocator;
	STD vector<FMOD::Channel*>		m_SoundChannel;
	STD vector<FMOD::Sound*>		m_Sounds;
};