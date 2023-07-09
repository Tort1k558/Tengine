#pragma once

#include<chrono>

class Timer
{
public:
	Timer() = delete;
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	static void Start();
	static double GetDeltaTime();
private:
	static std::chrono::high_resolution_clock::time_point m_startTime;
};