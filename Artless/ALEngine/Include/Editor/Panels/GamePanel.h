/*!
file:	GamePanel.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the GamePanel class.
		The GamePanel class contains information and functions necessary for
		the Game Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef GAME_PANEL_H
#define GAME_PANEL_H

#if EDITOR

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
		Math::Vec2 GetMouseWorldPos(void);

		/*!*********************************************************************************
			\brief
			Returns the cursor's screen space with respect to the Game Panel.

			\return
			Cursor Screen Space position with respect to the Game Panel.
		***********************************************************************************/
		Math::Vec2 GetMousePosWRTPanel(void);

		/*!*********************************************************************************
			\brief
			Returns the ImGui screen position of a vector that is in World Space.

			\param [in] pos
			World Space position, to be converted to screen space

			\return
			Returns the screen position of a vector that is in World Space.
			Returns numeric limits if out of Panel.
		***********************************************************************************/
		Math::Vec2 WorldToScreenPosVec(Math::Vec2 pos);

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

		/*!*********************************************************************************
			\brief
			Gets the Width of the Scene Panel

			\return
			Returns the selected Entity
		***********************************************************************************/
		f64 GetGamePanelWidth(void);

		/*!*********************************************************************************
			\brief
			Gets the Height of the Scene Panel

			\return
			Returns the selected Entity
		***********************************************************************************/
		f64 GetGamePanelHeight(void);

	private:		
		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

		// Panel Size
		f32 m_PanelWidth{ 0 }, m_PanelHeight{ 0 };

		ImVec2 m_ImGuiMousePos{};
		ImVec2 m_ImGuiPanelPos{};


	};
}

#endif

#endif