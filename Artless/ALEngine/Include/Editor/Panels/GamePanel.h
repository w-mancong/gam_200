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

		/*!*********************************************************************************
			\brief
			Returns the cursor's world space position.

			\return
			Cursor World Space position.
			Returns a Vec2 containing std::numeric_limits::max for x and y if the mouse
			position was handled outside of the Scene viewport
		***********************************************************************************/
		Math::Vec2 GetMouseWorldPos();

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
		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

		// Scene Size
		f32 m_SceneWidth{ 0 }, m_SceneHeight{ 0 };

		ImVec2 m_ImGuiMousePos{};
		ImVec2 m_ImGuiPanelPos{};
	};
}

#endif