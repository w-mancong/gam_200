/*!
file:		Logger.h
author:		Lucas Nguyen
email:		l.nguyen@digipen.edu
brief:		Contains a logger for the project with the use of spdlog.
			More info on spdlog can be found here: https://github.com/gabime/spdlog

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ALEngine::Exceptions
{
	class Logger
	{
	public:
		Logger() = default;
		~Logger() = default;

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}

		static void Init();
		void Exit();
		

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

#define LOGGER_NAME "AL Logger"

// SET THE MIN LEVEL FOR LOG OUTPUT. (CRITICAL > ERROR > WARN > INFO > DEBUG > TRACE)
#define AL_CORE_SET_LEVEL(...) ::ALEngine::Exceptions::Logger::GetCoreLogger()->set_level(__VA_ARGS__) 

// Set the pattern to be used for log output
#define AL_CORE_SET_PATTERN(...) ::spdlog::set_pattern(__VA_ARGS__)

// Set the Color (Text) to be used for log output
#define AL_CORE_SET_COLOR_MT(...) ::spdlog::stderr_color_mt(__VA_ARGS__)

// Definitions for all the log outputs
#define AL_CORE_TRACE(...)		::ALEngine::Exceptions::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define AL_CORE_DEBUG(...)		::ALEngine::Exceptions::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define AL_CORE_INFO(...)		::ALEngine::Exceptions::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define AL_CORE_WARN(...)		::ALEngine::Exceptions::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define AL_CORE_ERROR(...)		::ALEngine::Exceptions::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define AL_CORE_CRITICAL(...)	::ALEngine::Exceptions::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Definitions for all the level_enum in common.h (spdlog includes)
#define AL_TRACE	::spdlog::level::level_enum::trace
#define AL_DEBUG	::spdlog::level::level_enum::debug
#define AL_INFO		::spdlog::level::level_enum::info
#define AL_WARN		::spdlog::level::level_enum::warn
#define AL_ERROR	::spdlog::level::level_enum::err
#define AL_CRITICAL ::spdlog::level::level_enum::critical
#define AL_OFF		::spdlog::level::level_enum::off

// To shut down the logger 
#define AL_LOGGER_SHUT_DOWN ::spdlog::shutdown()

#endif