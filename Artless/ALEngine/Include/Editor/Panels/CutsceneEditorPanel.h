/*!
file:	CutsceneEditorPanel.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the CutsceneEditorPanel class.
		The CutsceneEditorPanel class contains a Cutscene Editor that edits cutscenes 
		and cutscene sequences.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef CUTSCENE_EDITOR_PANEL_H
#define CUTSCENE_EDITOR_PANEL_H

#if EDITOR

namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
			Class that aids in the display of the Cutscene Editor Panel for the editor
	***********************************************************************************/
	class CutsceneEditorPanel
	{
	public:
		/*!*********************************************************************************
			\brief
				Default constructor for the CutsceneEditorPanel
		***********************************************************************************/
		CutsceneEditorPanel(void);

		/*!*********************************************************************************
			\brief
				Default destructor for the CutsceneEditorPanel
		***********************************************************************************/
		~CutsceneEditorPanel(void);

		/*!*********************************************************************************
			\brief
				Updates the Cutscene Editor Panel
		***********************************************************************************/
		void OnImGuiRender(void);

		/*!*********************************************************************************
			\brief
				Updates the Cutscene Editor Panel
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief
			Updates the menu bar
		***********************************************************************************/
		void UpdateMenuBar(void);

		/*!*********************************************************************************
			\brief
				Set whether the panel is open
			\param isOpen
				Whether the panel is open
		***********************************************************************************/
		void SetPanelIsOpen(b8 isOpen);

		/*!*********************************************************************************
			\brief
				Set whether the panel is open
			\param isOpen
				Whether the panel is open
		***********************************************************************************/
		b8 GetPanelIsOpen(void);

	private:
		// Variables
		b8 m_PanelIsOpen{ false };						// Keeps track of whether the Panel is open
		std::string m_SelectedSequence{};				// Name of the selected sequence
		u32 m_SelectedCutsceneIndex{};					// Index of selected cutscene
	};
}

#endif

#endif