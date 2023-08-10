#pragma once

#include<chrono>

#include"Core/Core.h"

namespace Tengine
{
	class TENGINE Timer
	{
	public:
		Timer() = delete;
		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;
		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) = delete;

		static void Start();
		static void End();
		static std::chrono::high_resolution_clock::time_point GetStartPoint();
		static std::chrono::high_resolution_clock::time_point GetNowPoint();
		static double GetDeltaTime();
		static double GetDifferenceBetweenPoints(std::chrono::high_resolution_clock::time_point point1, std::chrono::high_resolution_clock::time_point point2);
		static void SetDeltaTime(double delta);
	private:
		static std::chrono::high_resolution_clock::time_point m_startTime;
		static double m_delta;
	};
}