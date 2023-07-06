#include "Timer.h"

std::chrono::high_resolution_clock::time_point Timer::m_startTime;
void Timer::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

double Timer::GetDeltaTime()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double>(currentTime - m_startTime).count();
}
