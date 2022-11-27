/*!
file:	SceneHierarchyPanel.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the SceneHierarchPanel class.
		The SceneHierarchyPanel class contains information and functions necessary for
		the Scene Hierarchy Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H

#if EDITOR

namespace ALEngine::Editor
{	
	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Scene Hierarcy Panel for the editor
	***********************************************************************************/
	class SceneHierarchyPanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the SceneHierarchyPanel class
		***********************************************************************************/
		SceneHierarchyPanel(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the SceneHierarchyPanel class
		***********************************************************************************/
		~SceneHierarchyPanel(void) {}

		/*!*********************************************************************************
			\brief
			Updates the Scene Hierarchy Panel 
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
		void SetDefaults(Math::Vec2 pos, Math::Vec2 size);

	private:
		/*!*********************************************************************************
			\brief
			Updates the entity for the Scene Hierarchy Panel
			\param child
			Child entity to be updated onto the Scene Hierarchy Panel
			\param popup_hasopen
			Boolean for whether there is a popup
		***********************************************************************************/
		void UpdateEntitySHP(ECS::Entity child, b8& popup_hasopen);

		/*!*********************************************************************************
			\brief
			Updates the Menu Bar for the Scene Hierarchy Panel
		***********************************************************************************/
		void UpdateMenuBar(void);

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

		// Pointer to the list of entities
		const ECS::EntityList *m_EntityList;

		// Entity Selected by Hover
		ECS::Entity m_EntityHover{ ECS::MAX_ENTITIES };
	};
}

#endif

#endif