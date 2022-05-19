/*!
file:		Time.cpp
author:		Wong Man Cong
co-author:	Lucas Nguyen
email:		w.mancong@digipen.edu
			l.nguyen@digipen.edu
brief:		This file contains a class "Time" that acts as the framerate controller.
			The delta time and FPS are calculated here.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "Utility/Time.h"
#include "pch.h"

//namespace
//{
//	f64 const NANO_TO_SECONDS = 1'000'000'000.0;
//	u64 const MAX_SAMPLES = 100;
//
//	u64 tickIndex = 0;
//	float tickSum = 0;
//	float tickList[MAX_SAMPLES];
//	/* need to zero out the ticklist array before starting */
//	/* average will ramp up until the buffer is full */
//	/* returns average ticks per frame over the MAXSAMPLES last frames */
//	float CalcAverageTick(float newTick)
//	{
//		tickSum -= tickList[tickIndex];  /* subtract value falling off */
//		tickSum += newTick;              /* add new value */
//		tickList[tickIndex] = newTick;   /* save new value so it can be subtracted later */
//		(++tickIndex) %= MAX_SAMPLES;
//		/* return average */
//		return(tickSum / MAX_SAMPLES);
//	}
//}

namespace ManCong
{
	namespace Utility
	{
		static constexpr long NUM_NANO_IN_SEC = 1000000000;

		// Static member variables definition
		f32 Time::m_DeltaTime = 0.0f, Time::m_FPS = 0.0f;

		int Time::m_TargetFPS = 0;	// 0 means unlimited
		bool Time::m_HasFPSLimit = false;

		nanoseconds Time::m_Ticks[MAX_SAMPLES];
		nanoseconds Time::m_TickSum = nanoseconds::zero();
		int Time::m_TickIndex = 0;

		hd_clock::time_point Time::m_StartTime = hd_clock::now();	// Set Application start time
		std::chrono::steady_clock::time_point Time::m_ClockedTime{};

		/*!*********************************************************************************
			\brief
			Constructor for Time class.
		***********************************************************************************/
		Time::Time()
		{
			for (int i = 0; i < MAX_SAMPLES; ++i)
				m_Ticks[i] = nanoseconds::zero();

			m_TickSum = nanoseconds::zero();
			m_TickIndex = 0;

			// Set Target FPS (Next time can read from file instead)
			m_TargetFPS = 60;	// 60 FPS
			m_HasFPSLimit = true;
		}
		/*
		void Time::Init(void)
		{
			m_ClockedTime = std::chrono::high_resolution_clock::now();
		}

		void Time::Update(void)
		{
			auto newTime = std::chrono::high_resolution_clock::now();
			m_DeltaTime = static_cast<f32>((newTime - m_ClockedTime).count()) / static_cast<f32>(NANO_TO_SECONDS);
			m_FPS = CalcAverageTick(1.0f / m_DeltaTime);
			m_ClockedTime = newTime;
		}*/

		/*!*********************************************************************************
			\brief
			Clock the current time.
			Should be called just before calling update!
		***********************************************************************************/
		void Time::ClockTimeNow()
		{
			m_ClockedTime = hd_clock::now();
		}

		/*!*********************************************************************************
			\brief
			Calculates the delta time and makes the main thread wait according to the
			FPS limit.
		***********************************************************************************/
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
			m_DeltaTime = static_cast<float>(static_cast<double>(time_diff.count()) / static_cast<double>(NUM_NANO_IN_SEC));

			// Calculate the number of ticks in the last 100 frames
			m_TickSum -= m_Ticks[m_TickIndex];	// Subtract tick value falling off
			m_TickSum += time_diff;				// Add new tick value
			m_Ticks[m_TickIndex] = time_diff;	// Update new tick value

			(++m_TickIndex) %= MAX_SAMPLES;

			// Calculate current FPS
			if (m_Ticks[MAX_SAMPLES - 1] != nanoseconds::zero())
				m_FPS = static_cast<float>(1.0 / ((m_TickSum / MAX_SAMPLES).count() / static_cast<double>(NUM_NANO_IN_SEC)));
			else
				m_FPS = static_cast<float>(1.0 / ((m_TickSum / m_TickIndex).count() / static_cast<double>(NUM_NANO_IN_SEC)));
		}

		/*!*********************************************************************************
			\brief
			Set the Target FPS.
		***********************************************************************************/
		void Time::SetTargetFPS(int _target)
		{
			m_TargetFPS = _target;
			m_HasFPSLimit = _target ? true : false;
		}
	}
}