/*!
file:		Time.h
author:		Wong Man Cong
co-author:	Lucas Nguyen
email:		w.mancong@digipen.edu
			l.nguyen@digipen.edu
brief:		This file contain a static class declaration to calculating the dt, fps 
			and fixedTime of the program.
			This file contains a class "Time" that acts as the framerate controller

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TIME_H
#define TIME_H

#include <chrono>	// Time library
#include "Type.h"

static constexpr s32 MAX_SAMPLES = 100;

namespace ManCong
{
	namespace Utility
	{
		using hd_clock = std::chrono::high_resolution_clock;

		class Time
		{
		public:
			/*!*********************************************************************************
				\brief
				Initializes the timer to calculate the delta time every frame
			***********************************************************************************/
			static void Init(void);

			/*!*********************************************************************************
				\brief
				Clock the current time.
				Should be called just before calling update!
			***********************************************************************************/
			static void ClockTimeNow();

			/*!*********************************************************************************
				\brief
				Calculates the delta time and makes the main thread wait according to the
				FPS limit.
			***********************************************************************************/
			static void WaitUntil();

			/*!*********************************************************************************
				\brief
				Set the Target FPS.
			***********************************************************************************/
			static void SetTargetFPS(s32 _target);

		public:
			static f32 m_DeltaTime;			// Delta Time
			static const f32 m_FixedDeltaTime;	// Fixed Delta Time
			static f32 m_FPS;				// Current FPS

		private:
			/*!*********************************************************************************
				\brief
				Constructor for Time class.
			***********************************************************************************/
			Time(void);

			/*!*********************************************************************************
				\brief
				Destructor for Time class.
			***********************************************************************************/
			~Time(void) = default;

			static s32 m_TargetFPS;		// Target FPS
			static bool m_HasFPSLimit;	// Whether there is an FPS limit

			static std::chrono::nanoseconds m_Ticks[MAX_SAMPLES];		// Stores the number of ticks in the past 100 frames
			static std::chrono::nanoseconds m_TickSum;				// Number of ticks
			static s32 m_TickIndex;						// Index for Ticks

			static hd_clock::time_point m_StartTime;	// Start time of the application
			static hd_clock::time_point m_ClockedTime;	// Time that was last clocked
		};
	}
}

#endif