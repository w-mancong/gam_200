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