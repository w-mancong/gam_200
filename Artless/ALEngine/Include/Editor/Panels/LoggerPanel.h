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
	private:
		std::ostringstream m_LoggerOSS;		// Logger's ostringtream
		u32 m_LogFlags{ 0 };				// Flag for which level to display

		// Panel size
		const ImVec2 PANEL_MIN{ 500, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
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
}

#endif