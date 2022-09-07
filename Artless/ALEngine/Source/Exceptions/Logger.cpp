#include "pch.h"

void ALEngine::Exceptions::Logger::Init()
{
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	// Set the patter for printing to console
	consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.&e] %v%$");

	std::vector<spdlog::sink_ptr> sinks{ consoleSink };
	auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sinks.begin(), sinks.end());
	// Set level to trace, basically lowest level to print to console
	logger->set_level(spdlog::level::trace);
	// Set flush level to trace, basically only print err onwards out to file
	logger->flush_on(spdlog::level::err);
	// Register logger
	spdlog::register_logger(logger);
}

void ALEngine::Exceptions::Logger::Exit()
{
	// Shutdown spdlog
	spdlog::shutdown();
}