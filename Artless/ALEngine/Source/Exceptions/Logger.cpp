#include "pch.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define OUT_FILE "logs/debug_log.txt"
#define MAX_OUTPUT_SIZE (1048576 * 5)

namespace ALEngine::Exceptions
{
	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;

	void Logger::Init()
	{
		//auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		// Set the patter for printing to console
		//consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.&e] %v%$");
		//consoleSink->set_pattern("%^[%D %T] %n: %v%$");
		//spdlog::set_pattern("%^[%D %T] %n: %v%$");
		AL_CORE_SET_PATTERN("%^[%n] [%l] [%D %T] %v%$");

		//std::vector<spdlog::sink_ptr> sinks{ consoleSink };
		//auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sinks.begin(), sinks.end());
		//// Set level to trace, basically lowest level to print to console
		//logger->set_level(spdlog::level::trace);
		//// Set flush level to trace, basically only print err onwards out to file
		//logger->flush_on(spdlog::level::err);
		//// Register logger
		//spdlog::register_logger(logger);

		s_CoreLogger = AL_CORE_SET_COLOR_MT("AL");
		//s_CoreLogger->set_level(spdlog::level::trace);
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