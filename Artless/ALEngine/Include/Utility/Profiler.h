/*!
file:	Profiler.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the Profiler class.
		The Profiler keeps track of the engine's performance data.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef PROFILER_H
#define PROFILER_H

namespace ALEngine::Utility
{
	/*!*********************************************************************************
		\brief
		Struct that contains the Timer Data for the engine's performance for each 
		process
	***********************************************************************************/
	struct TimerData
	{
		std::string m_Name;	// Process Name
		f32 m_Time;			// Time taken for process to be completed
	};

	/*!*********************************************************************************
		\brief
		Class that keeps track of the engine's performance data
	***********************************************************************************/
	class Profiler
	{
	public:
		/*!*********************************************************************************
			\brief
			Gets the list of TimerData from the profiler

			\return
			Returns the list of TimerData
		***********************************************************************************/
		static const std::vector<TimerData>& GetTimerDataList(void);

		/*!*********************************************************************************
			\brief
			Adds a TimerData info to the list of TimeData

			\param [in] data
			The TimerData info to be added to the profiler's list
		***********************************************************************************/
		static void AddTimerData(TimerData data);

		/*!*********************************************************************************
			\brief
			Clears the TimerData list
		***********************************************************************************/
		static void ClearTimerData(void);

	private:
		/*!*********************************************************************************
			\brief
			Default constructor for the profiler
		***********************************************************************************/
		Profiler(void) {};

		/*!*********************************************************************************
			\brief
			Default destructor for the profiler
		***********************************************************************************/
		~Profiler(void) = default;

		// List of timer data
		static std::vector<TimerData> m_TimerDataList;
	};
}

		/*!*********************************************************************************
			\brief
			Macro function for profiler timer per process

			\param [in] name
			The name of the process being recorded
		***********************************************************************************/
#define PROFILER_TIMER(name) Timer ProfilerTimer##__LINE__(name)

#endif