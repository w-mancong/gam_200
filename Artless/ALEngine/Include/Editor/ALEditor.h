/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the ALEditor class.
		This is where most of the ImGui related code will be done.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_EDITOR_H
#define AL_EDITOR_H

#include  <Editor/Panels/ContentBrowserPanel.h>
namespace ALEngine::Editor
{
	class ALEditor : public Templates::Singleton<ALEditor>
	{
	public:
		/*!*********************************************************************************
			\brief
			Updates the editor each frame.
			Mainly updates each ImGui panel
		***********************************************************************************/
		void Update();

		/*!*********************************************************************************
			\brief
			Used to shutdown and delete all necessary ImGui related processes.
		***********************************************************************************/
		void Exit();

		/*!*********************************************************************************
			\brief
			Signals the start of a new ImGui frame; any ImGui code is sandwiched between 
			this and End()
			Ensure to call at the start of each frame, before any ImGui code is called
		***********************************************************************************/
		void Begin();

		/*!*********************************************************************************
			\brief
			Signals the end of an ImGui frame; any ImGui code is sandwiched between
			this and Begin()
			Ensure to call at the end of each frame, after all objects are rendered, but
			before framebuffers get swapped.
		***********************************************************************************/
		void End();

		/*!*********************************************************************************
			\brief
			Sets ImGui to be enabled or disabled.

			\param [in] isEnabled
			Boolean for whether the ImGui will be set to enabled or disabled
		***********************************************************************************/
		void SetImGuiEnabled(b8 isEnabled) { m_ImGuiEnabled = isEnabled; }

		/*!*********************************************************************************
			\brief
			Sets ImGui to be enabled or disabled.

			\param [in] isEnabled
			Boolean for whether the ImGui will be set to enabled or disabled
		***********************************************************************************/
		b8 GetImGuiEnabled() { return m_ImGuiEnabled; }

		/*!*********************************************************************************
			\brief
			Sets ImGui Docking to be enabled or disabled.

			\param [in] isEnabled
			Boolean for whether the ImGui Docking will be set to enabled or disabled
		***********************************************************************************/
		void SetDockingEnabled(b8 isEnabled) { m_DockingEnabled = isEnabled; }

		/*!*********************************************************************************
			\brief
			Sets the entity to be selected by the ImGui inspector

			\param [in] setter
			Pointer to the transform of the entity to be selected by te inspector.
			Soon It will be based on entity instead of just transform alone
		***********************************************************************************/
		void SetSelectedEntityTransform(ECS::Entity setter) { 
			imguizmo_panel.SetSelectedEntityTransform(setter); };

	private:
		/*!*********************************************************************************
			\brief
			Constructor for the ALEditor class
		***********************************************************************************/
		ALEditor(void) { Init(); }

		/*!*********************************************************************************
			\brief
			Destructor for the ALEditor class
		***********************************************************************************/
		~ALEditor(void) = default;

		/*!*********************************************************************************
			\brief
			Initialization for the ImGui editor
		***********************************************************************************/
		void Init();

		/*!*********************************************************************************
			\brief
			Runs code that enables docking for ImGui windows/panels.
			Is to be called every frame when Docking is enabled
		***********************************************************************************/
		void Docking();

		friend class Templates::Singleton<ALEditor>;
		friend class Memory::StaticMemory;

		// Window Min Size
		ImVec2 m_WinMinSize{ 320, 350 };

		// Variables
		b8 m_ImGuiEnabled{ false };
		b8 m_DockingEnabled{ false };	// Set to true if docking is to be enabled

		// Panels
		ContentBrowserPanel cbp;
		InspectorPanel imguizmo_panel;
		LoggerPanel logger_panel;
		SceneHierarchyPanel hierarchy_panel;
	};
}

#endif