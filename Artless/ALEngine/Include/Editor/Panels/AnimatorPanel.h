/*!
file:	AnimatorPanel.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the ANimatorPanel class.
		The AnimatorPanel class contains an Animator Editor that edits the number of
		frames for each frame within the Animation

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef ANIMATOR_PANEL_H
#define ANIMATOR_PANEL_H

namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
			Class that aids in the display of the Animator Panel for the editor
	***********************************************************************************/
	class AnimatorPanel
	{
	public:
		/*!*********************************************************************************
			\brief
				Default constructor for the AnimatorPanel
		***********************************************************************************/
		AnimatorPanel(void);

		/*!*********************************************************************************
			\brief
				Default destructor for the AnimatorPanel
		***********************************************************************************/
		~AnimatorPanel(void);

		/*!*********************************************************************************
			\brief
				Updates the Animator Panel
		***********************************************************************************/
		void OnImGuiRender(void);

		/*!*********************************************************************************
			\brief
				Set the Selected Animator
			\param [in] animatorSelected
				Animator to be Selected
		***********************************************************************************/
		void SetSelectedAnimator(Animator& animatorSelected);

		/*!*********************************************************************************
			\brief
				Set the Selected Animation
			\param [in] animationSelected
				Animation to be Selected
		***********************************************************************************/
		void SetSelectedAnimation(Animation& animationSelected);

		/*!*********************************************************************************
			\brief
				Set whether the Animator Panel is open
			\param [in] panelIsOpen
				Boolean for whether Panel is Open
		***********************************************************************************/
		void SetPanelIsOpen(b8 panelIsOpen);

		/*!*********************************************************************************
			\brief
				Get whether the Animator Panel is open
			\return
				Returns true if Panel is Open,
				Else returns false
		***********************************************************************************/
		b8 GetPanelIsOpen(void);

		/*!*********************************************************************************
			\brief
				Set the panel's minimum size
			\param min
				Panel's min size
		***********************************************************************************/
		void SetPanelMin(Math::Vec2 min);

	private:
		/*!*********************************************************************************
			\brief
			Updates the current map for Animator Panel
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief
			Updates the menu bar
		***********************************************************************************/
		void UpdateMenuBar(void);

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Hierarchy for Animator/Animation: One Player -> One Animator -> Multiple Animations
		// i.e. Player Animator contains "PlayerMove", "PlayerIdle" & "PlayerAttack" Animations
		Animator m_SelectedAnimator{};			// Reference to the selected Animator
		Animation m_SelectedAnimation{};		// Reference to the selected Animation
		Math::Vec2Int m_SelectedFrame{};		// The X and Y of the selected frame
		u32 m_SelectedAnimatorIndex{ 0 };		// Selected Animator Index
		u32 m_SelectedAnimationIndex{ 0 };		//

		b8 m_PanelIsOpen{ false };				// Keeps track of whether the Panel is open
		std::string m_CurrClipName{},			// Current Clip Name
			m_OldCurrClipName{};				// Current Clip Name but old
	};
}

#endif