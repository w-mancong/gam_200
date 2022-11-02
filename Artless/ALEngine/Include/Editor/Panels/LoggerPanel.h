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
		void SetDefault(Math::Vec2 pos, Math::Vec2 size);
	private:
		std::ostringstream m_LoggerOSS;		// Logger's ostringtream
		u32 m_LogFlags{ 0 };				// Flag for which level to display

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size
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