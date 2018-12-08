#pragma once
#include <chrono>

class Timer
{
	using TimeDuration = STD chrono::duration<float>;
	using Time = STD chrono::high_resolution_clock;
	using TimePoint = Time::time_point;

public:
	Timer();
	~Timer();

	void  Tick();
	bool  FlushAccumulatedTime();
	void  Reset();
	float Interpolation() const;

private:
	TimePoint	m_CurrentTime;
	TimePoint	m_PreviousTime;
	float		m_TimeFrame;
	float		m_TimeAccumulator;
};

