/*!
file:	SceneBuildOrderPanel.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the SceneBuildOrderPanel class.
		The InspectorPanel class contains information and functions necessary for
		the editing the Build Order of scenes.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef SCENE_BUILD_ORDER_PANEL_H
#define SCENE_BUILD_ORDER_PANEL_H

#if EDITOR

namespace ALEngine::Editor
{
	class SceneBuildOrderPanel
	{
	public:
		/*!*********************************************************************************
			\brief
				Default constructor for the SceneBuildOrderPanel
		***********************************************************************************/
		SceneBuildOrderPanel(void);

		/*!*********************************************************************************
			\brief
				Default destructor for the SceneBuildOrderPanel
		***********************************************************************************/
		~SceneBuildOrderPanel(void);

		/*!*********************************************************************************
			\brief
				Updates the Scene Build Order Panel
		***********************************************************************************/
		void OnImGuiRender(void);

		/*!*********************************************************************************
			\brief
				Set if the Panel is Open
			\param [in] isOpen
				Boolean to set whether the panel is open
		***********************************************************************************/
		void SetPanelIsOpen(bool isOpen);

		/*!*********************************************************************************
			\brief
				Get if the Panel is Open
			\return
				Returns true if Panel is Open,
				else returns false
		***********************************************************************************/
		bool GetPanelIsOpen(void);

	private:
		bool m_PanelIsOpen{ false };
	};
}

#endif
#endif