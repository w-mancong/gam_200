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

		static void Init();
		void Exit();

	};
}

#define LOGGER_NAME "WowwLogger"

#endif