/*!
file:		Time.cpp
author:		Lucas Nguyen
email:		l.nguyen@digipen.edu
brief:		This file contains a class "Time" that acts as the framerate controller.
			The delta time and FPS are calculated here.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Utility
{
	using namespace std::chrono;
	static constexpr long NUM_NANO_IN_SEC = 1000000000;

	// Static member variables definition
	f32 Time::m_DeltaTime = 0.0f, 
		Time::m_FPS = 0.0f;
	const f32 Time::m_FixedDeltaTime = 1.f / 60.f;	// Fixed delta time is 60 fps

	s32 Time::m_TargetFPS = 0;	// 0 means unlimited
	bool Time::m_HasFPSLimit = false;

	nanoseconds Time::m_Ticks[MAX_SAMPLES];
	nanoseconds Time::m_TickSum = nanoseconds::zero();
	s32 Time::m_TickIndex = 0;

	hd_clock::time_point Time::m_StartTime = hd_clock::now();	// Set Application start time
	steady_clock::time_point Time::m_ClockedTime{};

	void Time::Init()
	{
		for (s32 i = 0; i < MAX_SAMPLES; ++i)
			m_Ticks[i] = nanoseconds::zero();

		m_TickSum = nanoseconds::zero();
		m_TickIndex = 0;

		// Set Target FPS (Next time can read from file instead)
		m_TargetFPS = 60;	// 60 FPS
		m_HasFPSLimit = true;
	}

	void Time::ClockTimeNow()
	{
		m_ClockedTime = hd_clock::now();
	}

	void Time::WaitUntil()
	{
		auto time_diff = hd_clock::now() - m_ClockedTime;
		const auto ideal_wait_time = nanoseconds(NUM_NANO_IN_SEC / Time::m_TargetFPS);

		// If there is an FPS limit, wait
		if (m_HasFPSLimit)
		{
			// Loop until enough time has passed
			while (hd_clock::now() - m_ClockedTime < ideal_wait_time) { /*empty loopings*/ }

			// Get time diff after wait
			time_diff = hd_clock::now() - m_ClockedTime;
		}

		// Calculate Delta Time
		m_DeltaTime = static_cast<float>(static_cast<f64>(time_diff.count()) / static_cast<f64>(NUM_NANO_IN_SEC));

		// Calculate the number of ticks in the last 100 frames
		m_TickSum -= m_Ticks[m_TickIndex];	// Subtract tick value falling off
		m_TickSum += time_diff;				// Add new tick value
		m_Ticks[m_TickIndex] = time_diff;	// Update new tick value

		(++m_TickIndex) %= MAX_SAMPLES;

		// Calculate current FPS
		if (m_Ticks[MAX_SAMPLES - 1] != nanoseconds::zero())
			m_FPS = static_cast<f32>(1.0 / ((m_TickSum / MAX_SAMPLES).count() / static_cast<f64>(NUM_NANO_IN_SEC)));
		else
			m_FPS = static_cast<f32>(1.0 / ((m_TickSum / m_TickIndex).count() / static_cast<f64>(NUM_NANO_IN_SEC)));
	}

	void Time::SetTargetFPS(s32 _target)
	{
		m_TargetFPS = _target;
		m_HasFPSLimit = _target ? true : false;
	}
}