#pragma once

#include<chrono>

class Timer
{
public:
	static void Start();
	static double GetDeltaTime();
private:
	static double m_deltaTime;
	static std::chrono::high_resolution_clock::time_point m_startTime;
};