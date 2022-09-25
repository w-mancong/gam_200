/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the LoggerPanel class

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef LOGGER_PANEL_H
#define LOGGER_PANEL_H

namespace ALEngine::Editor
{
	class LoggerPanel
	{
	public:
		LoggerPanel();
		~LoggerPanel();

		void OnImGuiRender();
	private:
		std::ostringstream logger_oss;
		u32 log_flags{ 0 };

		// Panel size
		const ImVec2 PANEL_MIN{ 500, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
	};

	enum class LOG_FLAGS
	{
		LOG_NONE		= 0,
		LOG_TRACE		= 1 << 0,
		LOG_DEBUG		= 1 << 1,
		LOG_INFO		= 1 << 2,
		LOG_WARN		= 1 << 3,
		LOG_ERROR		= 1 << 4,
		LOG_CRITICAL	= 1 << 5,
		
		LOG_ALL			= LOG_TRACE | LOG_DEBUG | LOG_INFO | LOG_WARN | LOG_ERROR | LOG_CRITICAL
	};
}

#endif