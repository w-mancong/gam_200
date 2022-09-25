/*!
file:		Time.h
author:		Lucas Nguyen
email:		l.nguyen@digipen.edu
brief:		This file contains a class "Time" that acts as the framerate controller.
			The delta time and FPS are calculated here.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TIME_H
#define TIME_H

#include <chrono>	// Time library
#include "Type.h"

static constexpr s32 MAX_SAMPLES = 100;

namespace ALEngine
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
			static void ClockTimeNow(void);

			/*!*********************************************************************************
				\brief
				Calculates the delta time and makes the main thread wait according to the
				FPS limit.
			***********************************************************************************/
			static void WaitUntil(void);

			/*!*********************************************************************************
				\brief
				Set the Target FPS, which is the FPS the system will aim to hit

				\param [in] _target:
				Number to be set as the target FPS
			***********************************************************************************/
			static void SetTargetFPS(s32 _target);

		public:
			static f32 m_DeltaTime;				// Delta Time
			static const f32 m_FixedDeltaTime;	// Fixed Delta Time
			static f32 m_FPS;					// Current FPS

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