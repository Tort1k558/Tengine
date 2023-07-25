#include "Timer.h"

std::chrono::high_resolution_clock::time_point Timer::m_startTime;
double Timer::m_delta;

void Timer::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::End()
{
	m_delta = GetDifferenceBetweenPoints(GetNowPoint(), m_startTime);
}

std::chrono::high_resolution_clock::time_point Timer::GetNowPoint()
{
	return std::chrono::high_resolution_clock::now();
}

double Timer::GetDeltaTime()
{
	return m_delta;
}

double Timer::GetDifferenceBetweenPoints(std::chrono::high_resolution_clock::time_point point1, std::chrono::high_resolution_clock::time_point point2)
{
	return std::chrono::duration<double>(point1 - point2).count();
}

void Timer::SetDeltaTime(double delta)
{
	m_delta = delta;
}
