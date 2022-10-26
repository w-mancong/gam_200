/*!
file:	SceneHierarchyPanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the SceneHierarchPanel class.
		The SceneHierarchyPanel class contains information and functions necessary for
		the Scene Hierarchy Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef SCENE_HIERARCHY_PANEL_H
#define SCENE_HIERARCHY_PANEL_H

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
			Updates the entity for the Scene Hierarchy Panel
			\param child
			Child entity to be updated onto the Scene Hierarchy Panel
			\param popup_hasopen
			Boolean for whether there is a popup
		***********************************************************************************/
		void UpdateEntitySHP(ECS::Entity child, b8& popup_hasopen);

	private:
		// Panel sizes
		const ImVec2 PANEL_MIN{ 320, 350 };		// Min Size
		const ImVec2 PANEL_MAX{ 1920, 1080 };	// Max Size

		// Pointer to the list of entities
		const ECS::EntityList *m_EntityList;
	};
}

#endif