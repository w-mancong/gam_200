#include "pch.h"

namespace ALEngine::Utility
{
	Timer::Timer(const c8* _name) : m_Name(_name)
	{
		m_Stopped = false;
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		if (!m_Stopped)
			StopTimer();
	}

	void Timer::StopTimer()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		f32 dur = static_cast<f32>(end - start) * 0.001f;
	}
}