/*!
file:	ProfilerPanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the ProfilerPanel class.
		The ProfilerPanel class contains information and functions necessary for
		the Profiler Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef PROFILER_PANEL_H
#define PROFILER_PANEL_H

#if EDITOR

namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Profiler Panel for the editor
	***********************************************************************************/
	class ProfilerPanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the ProfilerPanel
		***********************************************************************************/
		ProfilerPanel(void) {};
		
		/*!*********************************************************************************
			\brief
			Default destructor for the ProfilerPanel
		***********************************************************************************/
		~ProfilerPanel(void) {};

		/*!*********************************************************************************
			\brief
			Updates the Profiler Panel
		***********************************************************************************/
		void OnImGuiRender(void);

	private:
		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };

		std::vector<Utility::TimerData> m_TempTimerDataList;	// Temp list of TimerData
																	// List gets cleared every 1s
	};
}

#endif

#endif