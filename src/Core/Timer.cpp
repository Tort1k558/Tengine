#include "Timer.h"

std::chrono::high_resolution_clock::time_point Timer::m_startTime;
double Timer::m_delta;

void Timer::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::End()
{
	m_delta = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_startTime).count();;
}

double Timer::GetDeltaTime()
{
	return m_delta;
}

void Timer::SetDeltaTime(double delta)
{
	m_delta = delta;
}
