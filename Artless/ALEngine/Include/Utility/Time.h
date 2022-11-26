/*!
file:		Time.h
author:		Lucas Nguyen
email:		l.nguyen@digipen.edu
brief:		This file contains a class "Time" that acts as the framerate controller.
			The delta time and FPS are calculated here.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TIME_H
#define TIME_H

static constexpr s32 MAX_SAMPLES = 100;	// Max number of frames to track the number of ticks

namespace ALEngine::Utility
{
	using hd_clock = std::chrono::high_resolution_clock;

	/*!*********************************************************************************
		\brief
		Class that keeps track of the engine's delta time and manages the engine's FPS
	***********************************************************************************/
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
			Set the Target FPS, which is the FPS the system will aim to hit.
			Set to 0 for Unlimited FPS

			\param [in] _target:
			Number to be set as the target FPS
		***********************************************************************************/
		static void SetTargetFPS(s32 _target);

		/*!*********************************************************************************
			\brief
			Set whether the engine has an FPS Limit

			\param [in] _hasLimit:
			Whether there is an FPS Limit or not
		***********************************************************************************/
		static void SetHasFPSLimit(b8 _hasLimit);

	public:
		static f32 m_DeltaTime;				// Delta Time
		static f32 m_Scale;					// Time Scale, to be used to change the scale of delta time
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

		static s32 m_TargetFPS;			// Target FPS
		static b8 m_HasFPSLimit;		// Whether there is an FPS limit
		static f32 m_ActualDeltaTime;	// The actual delta time

		static std::chrono::nanoseconds m_Ticks[MAX_SAMPLES];		// Stores the number of ticks in the past 100 frames
		static std::chrono::nanoseconds m_TickSum;				// Number of ticks
		static s32 m_TickIndex;						// Index for Ticks

		static hd_clock::time_point m_StartTime;	// Start time of the application
		static hd_clock::time_point m_ClockedTime;	// Time that was last clocked
	};

	//Max steps for fixed delta time for physics
	u32 constexpr MAX_STEP_FIXED_DT{ 3 };
}

#endif