/*!
file:	Logger.cpp
author:	Darrion Aw Wei Ting
email:	weitingdarrion.aw\@digipen.edu
brief:	This file contains a logger for the project with the use of spdlog.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/


#include "pch.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define OUT_FILE "logs/debug_log.txt"
#define MAX_OUTPUT_SIZE (1048576 * 5)

namespace ALEngine::Exceptions
{
	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;

	void Logger::Init()
	{
		AL_CORE_SET_PATTERN("%^[%n] [%l] [%D %T] %v%$");

		s_CoreLogger = AL_CORE_SET_COLOR_MT("AL");

		AL_CORE_SET_LEVEL(spdlog::level::trace);

		// Add sink for output file
		s_CoreLogger->sinks().push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(OUT_FILE, MAX_OUTPUT_SIZE, 10, true));

	}

	void Logger::Exit()
	{
		// Shutdown spdlog
		AL_LOGGER_SHUT_DOWN;
	}
}