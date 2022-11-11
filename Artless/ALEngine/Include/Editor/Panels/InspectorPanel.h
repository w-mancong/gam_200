/*!
file:	InspectorPanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the InspectorPanel class.
		The InspectorPanel class contains information and functions necessary for
		the Inspector Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef INSPECTOR_PANEL_H
#define INSPECTOR_PANEL_H

#ifdef EDITOR

namespace ALEngine::Editor
{
	// Predeclarations
	enum class InspectorComponents;

	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Inspector Panel for the editor
	***********************************************************************************/
	class InspectorPanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the InspectorPanel 
		***********************************************************************************/
		InspectorPanel(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the InspectorPanel
		***********************************************************************************/
		~InspectorPanel(void);

		/*!*********************************************************************************
			\brief
			Updates the Inspector Panel
		***********************************************************************************/
		void OnImGuiRender(void);

		/*!*********************************************************************************
			\brief
			Set the selected entity for the Inspector

			\param [in] setter
			Entity to be selected
		***********************************************************************************/
		void SetSelectedEntity(ECS::Entity setter);

		/*!*********************************************************************************
			\brief
			Get the selected entity for the Inspector

			\return
			Returns the selected Entity
		***********************************************************************************/
		const ECS::Entity GetSelectedEntity(void);

		/*!*********************************************************************************
			\brief
			Gets whether there is a selected entity

			\return
			Returns true if there is a selected entity, else returns false
		***********************************************************************************/
		bool HasSelectedEntity(void);
		
		/*!*********************************************************************************
			\brief
			Displays the Inspector Menu on the panel
		***********************************************************************************/
		void InspectorMenu(void);

		/*!*********************************************************************************
			\brief
			Displays the EntityData component info on the panel
		***********************************************************************************/
		void DisplayEntityData(void);

		/*!*********************************************************************************
			\brief
			Displays the Transform component info on the panel
		***********************************************************************************/
		void DisplayTransform(void);

		/*!*********************************************************************************
			\brief
			Displays the Sprite component info on the panel
		***********************************************************************************/
		void DisplaySprite(void);

		/*!*********************************************************************************
			\brief
			Displays the Sprite component info on the panel

			\return
			Current Gizmo Operation
		***********************************************************************************/
		ImGuizmo::OPERATION GetCurrGizmoOperation(void) const;

		/*!*********************************************************************************
			\brief
			Displays the Sprite component info on the panel

			\return
			Current Gizmo Operation
		***********************************************************************************/
		void AddComponentButton(void);

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
		static ImGuizmo::OPERATION m_CurrentGizmoOperation;	// Gizmo related, ImGuizmo
		ECS::Entity m_SelectedEntity{ ECS::MAX_ENTITIES };	// Entity Selected in Inspector

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

		// Selected Inspector
		InspectorComponents m_SelectedComponent{};
	};

	// Enum for different Components
	enum class InspectorComponents
	{
		InComp_EntityData = 0,
		InComp_Transform,
		InComp_Sprite,
		InComp_Total
	};
}

#endif

#endif