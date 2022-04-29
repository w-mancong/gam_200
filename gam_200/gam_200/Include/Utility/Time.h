/*!
file:	Time.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain a staic class declaration to calculating the dt, fps and fixedTime 
		of the program

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	TIME_H
#define TIME_H

namespace ManCong
{
	namespace Utility
	{
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
				To update the delta time every frame
			***********************************************************************************/
			static void Update(void);

			static f32 dt, fps, fixedTime;
		private:
			Time(void)	= default;
			~Time(void) = default;

			static std::chrono::steady_clock::time_point oldTime;
		};
	}
}

#endif