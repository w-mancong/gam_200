/*!
file:	TileEditorPanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the TileEditorPanel class.
		The Tile Editor Panel creates or loads previous Tile Levels created.
		The TileEditorPanel class contains information and functions necessary for
		the Tile Editor Panel of the editor to be displayed.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef TILE_EDITOR_PANEL_H
#define TILE_EDITOR_PANEL_H

namespace ALEngine::Editor
{
	// Forward Delcarations
	enum class TileType;

	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Tile Editor Panel for the editor
	***********************************************************************************/
	class TileEditorPanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the TileEditorPanel
		***********************************************************************************/
		TileEditorPanel(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the TileEditorPanel
		***********************************************************************************/
		~TileEditorPanel(void);

		/*!*********************************************************************************
			\brief
			Updates the Tile Editor Panel
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
		/*!*********************************************************************************
			\brief
			Updates the current map for tile editor
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief
			Updates the menu bar
		***********************************************************************************/
		void UpdateMenuBar(void);

		/*!*********************************************************************************
			\brief
			Creates a new Map for the Tile Editor
		***********************************************************************************/
		void CreateNewMap(void);

		/*!*********************************************************************************
			\brief
			Select Map for the Tile Editor
		***********************************************************************************/
		void SelectMap(void);

		/*!*********************************************************************************
			\brief
			Saves the current Map
			\param filePath
			File Path of the map to be saved
		***********************************************************************************/
		void SaveMap(const c8* filePath);

		/*!*********************************************************************************
			\brief
			Loads the Map
			\param filePath
			File Path of the map to be loaded
		***********************************************************************************/
		void LoadMap(const c8* filePath);

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Variables
		b8 m_HasMapLoaded{ false };				// Boolean to keep track of whether there is a map loaded
		b8 m_PanelIsOpen{ false };				// Keeps track of whether the Panel is open
		std::string m_FilePath{ "" };			// FilePath of the current file

		std::vector<std::vector<TileType>> m_TileMap{};			// Map of tiles
		s32 m_MapHeight{ 0 }, m_MapWidth{ 0 };					// Map Dimensions

		enum class LoadStage
		{
			CreateOrLoadSelection = 0,
			CreateMap,
			LoadMap,
		} m_CurrentLoadStage{ LoadStage::CreateOrLoadSelection };					// Keeps track of what section of loading of map the user is in
	};

	// Tile Editor Stuff
	enum class TileType
	{
		Empty = 0,
		Wall,
		Enemy
	};
}

#endif