/*!
file:	GamePanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the GamePanel class.
		The GamePanel class contains information and functions necessary for
		the Game Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef GAME_PANEL_H
#define GAME_PANEL_H

namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Game Panel for the editor
	***********************************************************************************/
	class GamePanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the GamePanel
		***********************************************************************************/
		GamePanel(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the GamePanel
		***********************************************************************************/
		~GamePanel(void);

		/*!*********************************************************************************
			\brief
			Updates the Game Panel
		***********************************************************************************/
		void OnImGuiRender(void);

	private:		
		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
	};
}

#endif