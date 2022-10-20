/*!
file:	ScenePanel.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the ScenePanel class.
		The ScenePanel class contains information and functions necessary for
		the Scene Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef SCENE_PANEL_H
#define SCENE_PANEL_H

namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
		Class that aids in the display of the Scene Panel for the editor
	***********************************************************************************/
	class ScenePanel
	{
	public:
		/*!*********************************************************************************
			\brief
			Default constructor for the ScenePanel
		***********************************************************************************/
		ScenePanel(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the ScenePanel
		***********************************************************************************/
		~ScenePanel(void);

		/*!*********************************************************************************
			\brief
			Updates the Scene Panel
		***********************************************************************************/
		void OnImGuiRender(void);

		/*!*********************************************************************************
			\brief
			Sets the current gizmo operation
			\param [in] _op:
			Current gizmo operation
		***********************************************************************************/
		void SetCurrentGizmoOperation(ImGuizmo::OPERATION _op);

		/*!*********************************************************************************
			\brief
			Sets the selected entity
			\param [in] _entt
			Selected Entity
		***********************************************************************************/
		void SetSelectedEntity(ECS::Entity _entt);

	private:
		static ImGuizmo::OPERATION m_CurrentGizmoOperation;	// Gizmo related, ImGuizmo
		ECS::Entity m_SelectedEntity{ ECS::MAX_ENTITIES }; // Entity Selected in Inspector

		// Panel size
		const ImVec2 PANEL_MIN{ 320, 350 };
		const ImVec2 PANEL_MAX{ 1920, 1080 };
	};
}
#endif