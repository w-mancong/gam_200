/*!
file:	LoggerPanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function definitions for the LoggerPanel class.
		The LoggerPanel class contains information and functions necessary for the
		Logger Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef LOGGER_PANEL_H
#define LOGGER_PANEL_H

#if _EDITOR
#include "spdlog/sinks/base_sink.h"

namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Logger Panel for the editor
	***********************************************************************************/
	class LoggerPanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the LoggerPanel class
		***********************************************************************************/
		LoggerPanel(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the LoggerPanel class
		***********************************************************************************/
		~LoggerPanel(void);

		/*!*********************************************************************************
			\brief
			Updates the Logger Panel
		***********************************************************************************/
		void OnImGuiRender(void);

		/*!*********************************************************************************
			\brief
			Set the panel's minimum size
			\param min
			Panel's min size
		***********************************************************************************/
		void SetPanelMin(Math::Vec2 min);

		/*!*********************************************************************************
			\brief
			Set the panel's default pos and size
			\param pos
			Panel's Pos
			\param size
			Panel's Size
		***********************************************************************************/
		void SetDefaults(Math::Vec2 pos, Math::Vec2 size);

	private:
		std::ostringstream m_LoggerOSS;		// Logger's ostringtream
		u32 m_LogFlags{ 0 };				// Flag for which level to display

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

	public:
		// List of messages
		std::vector<std::pair<std::string, s32>> m_MessageList;
	};

	/*!*********************************************************************************
		\brief
		enum class for Log's levels
	***********************************************************************************/
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

	/*!*********************************************************************************
		\brief
		Sink class to received messages from the spdlogger
	***********************************************************************************/
	template<typename Mutex>
	class ALSink : public spdlog::sinks::base_sink<Mutex>
	{
	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			m_Logger->m_MessageList.push_back(std::make_pair<std::string, s32>(fmt::to_string(formatted), static_cast<s32>(msg.level)));
		}

		void flush_() override
		{
			std::cout << std::endl;
		}
	public:
		LoggerPanel* m_Logger;

	};

	#include "spdlog/details/null_mutex.h"
	#include <mutex>
	using my_sink_mt = ALSink<std::mutex>;
	using my_sink_st = ALSink<spdlog::details::null_mutex>;
}

#endif

#endif