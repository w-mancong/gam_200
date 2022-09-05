/*!
file:	GLDebug.h
author:	
email:	
brief:	This file contains static class to log debugging messages

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	GLDEBUG_H
#define GLDEBUG_H

namespace ALEngine::Exceptions
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

	void APIENTRY glDebugOutput(GLenum source, GLenum type, u32 id, GLenum severity,
		GLsizei length, const s8* message, const void* userParam);
}

#endif