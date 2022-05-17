#include "pch.h"

namespace ManCong
{
	namespace Exceptions
	{
		/*!*********************************************************************************
			\brief
			Prints out an error message and force shut down the program
		***********************************************************************************/
		void DebugLog::Exit(const char* error_msg)
		{
			std::cerr << error_msg << std::endl;
			Memory::FreeAll();
			std::exit(EXIT_FAILURE);
		}
	}
}