/*!
file:		Timer.cpp
author:		Lucas Nguyen
email:		l.nguyen@digipen.edu
brief:		This file contains the definitions for the Timer class's functions.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Utility
{
	Timer::Timer(const c8* _name) : m_Name(_name)
	{
		m_Stopped = false;
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer(void)
	{
		if (!m_Stopped)
			StopTimer();
	}

	void Timer::StopTimer(void)
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		f32 dur = static_cast<f32>(end - start) * 0.001f;

		// Create new timer data
		TimerData newTD;
		newTD.m_Name = m_Name;
		newTD.m_Time = dur;

		Profiler::AddTimerData(newTD);
	}
}