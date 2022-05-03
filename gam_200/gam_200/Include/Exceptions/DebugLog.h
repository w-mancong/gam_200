/*!
file:	DebugLog.h
author:	
email:	
brief:	This file contains static class to log debugging messages

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	DEBUG_LOG_H
#define DEBUG_LOG_H

namespace ManCong
{
	namespace Exceptions
	{
		class DebugLog
		{
		public:
			/*!*********************************************************************************
				\brief
				Prints out an error message and force shut down the program
			***********************************************************************************/
			static void Exit(const char* error_msg);
		private:
			DebugLog(void) = default;
			~DebugLog(void) = default;
		};
	}
}

#endif