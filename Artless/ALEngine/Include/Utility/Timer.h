/*!
file:		Timer.h
author:		Lucas Nguyen
email:		l.nguyen\@digipen.edu
brief:		This file contains the declaration for the Timer class, which is used to 
			track the time taken for a function to execute, mainly used for the 
			Profiler.

			How this works is before a function is called, this class is created and 
			it will keep track of the time that it was constructed at. Once the 
			function ends, the Timer object will be destroyed, and in the destructor,
			the time of destruction is also tracked. Later, the difference between the
			start and end is calculated, getting the function's "lifetime". This data 
			is then pushed to the Profiler.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef TIMER_H
#define TIMER_H

namespace ALEngine::Utility
{
	/*!*********************************************************************************
		\brief
		Class that counts the time taken for a function to be executed
	***********************************************************************************/
	class Timer
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the Timer

			\param [in] _name
			Takes in the name of the process/function being tracked
		***********************************************************************************/
		Timer(const c8* _name);

		/*!*********************************************************************************
			\brief
			Default destructor for the Timer
		***********************************************************************************/
		~Timer(void);

		/*!*********************************************************************************
			\brief
			Stops the timer, and gets the time taken for the function to execute.
			Then pushes this data into the Profiler
		***********************************************************************************/
		void StopTimer(void);

	private:
		const c8* m_Name;	// Name of the function/process
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;	// Time stated
		b8 m_Stopped;	// Timer stopped or not
	};
}

#endif