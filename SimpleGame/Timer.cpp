#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Tick()
{
	m_CurrentTime = Time::now();
	m_TimeFrame = TimeDuration(m_CurrentTime - m_PreviousTime).count();
	m_PreviousTime = m_CurrentTime;
	m_TimeAccumulator += m_TimeFrame;
	m_ElapsedTime += m_TimeFrame;
}

float Timer::GetElapsedTime() const
{
	return m_ElapsedTime;
}

void Timer::ResetElapsedTime()
{
	m_ElapsedTime = 0.f;
}

bool Timer::FlushAccumulatedTime()
{
	bool Result = m_TimeAccumulator >= UPDATE_TIME;

	if(Result)
		m_TimeAccumulator -= UPDATE_TIME;

	return Result;
}

float Timer::Interpolation() const
{
	return m_TimeAccumulator * UPDATE_TIME;
}

void Timer::Reset()
{
	m_CurrentTime = Time::now();
	m_PreviousTime = Time::now();
	m_ElapsedTime = 0.f;
	m_TimeAccumulator = 0.f;
	m_TimeFrame = 0.f;
}
