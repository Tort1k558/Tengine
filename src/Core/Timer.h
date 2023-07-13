#pragma once

#include<chrono>

class Timer
{
public:
	Timer() = delete;
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	static void Start();
	static void End();
	static double GetDeltaTime();
	static void SetDeltaTime(double delta);
private:
	static std::chrono::high_resolution_clock::time_point m_startTime;
	static double m_delta;
};