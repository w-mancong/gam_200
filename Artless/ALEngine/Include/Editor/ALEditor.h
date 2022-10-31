/*!
file:	ALEditor.h
author:	Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains the function declarations for the ALEditor class.
		The ALEditor class essentially manages the Dear ImGui functions, as well as the
		different editor panels generated with the help of Dear ImGui.

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef AL_EDITOR_H
#define AL_EDITOR_H

#include  <Editor/Panels/ContentBrowserPanel.h>
namespace ALEngine::Editor
{
	/*!*********************************************************************************
		\brief
		Class that manages the Dear ImGui functions and editor panels generated with 
		the aid of Dear ImGui
	***********************************************************************************/
	class ALEditor : public Templates::Singleton<ALEditor>
	{
	public:
		/*!*********************************************************************************
			\brief
			Updates the editor each frame.
			Mainly updates each ImGui panel
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief
			Used to shutdown and delete all necessary ImGui related processes.
		***********************************************************************************/
		void Exit(void);

		/*!*********************************************************************************
			\brief
			Signals the start of a new ImGui frame; any ImGui code is sandwiched between 
			this and End()
			Ensure to call at the start of each frame, before any ImGui code is called
		***********************************************************************************/
		void Begin(void);

		/*!*********************************************************************************
			\brief
			Signals the end of an ImGui frame; any ImGui code is sandwiched between
			this and Begin()
			Ensure to call at the end of each frame, after all objects are rendered, but
			before framebuffers get swapped.
		***********************************************************************************/
		void End(void);

		/*!*********************************************************************************
			\brief
			Updates the Menu Bar for the Editor
		***********************************************************************************/
		void MainMenuBar(void);

		/*!*********************************************************************************
			\brief
			Sets ImGui to be enabled or disabled.

			\param [in] isEnabled
			Boolean for whether the ImGui will be set to enabled or disabled
		***********************************************************************************/
		void SetImGuiEnabled(b8 isEnabled);

		/*!*********************************************************************************
			\brief
			Get whether ImGui is to be enabled or disabled.

			\return
			Returns true if ImGui is enabled, otherwise returns false
		***********************************************************************************/
		b8 GetImGuiEnabled(void);

		/*!*********************************************************************************
			\brief
			Sets ImGui Docking to be enabled or disabled.

			\param [in] isEnabled
			Boolean for whether the ImGui Docking will be set to enabled or disabled
		***********************************************************************************/
		void SetDockingEnabled(b8 isEnabled);

		/*!*********************************************************************************
			\brief
			Sets the entity to be selected by the ImGui inspector

			\param [in] setter
			Pointer to the transform of the entity to be selected by te inspector.
			Soon It will be based on entity instead of just transform alone
		***********************************************************************************/
		void SetSelectedEntity(ECS::Entity setter);

		/*!*********************************************************************************
			\brief
			Gets the entity selected by the ImGui inspector

			\return
			Returns the selected Entity
		***********************************************************************************/
		const ECS::Entity GetSelectedEntity(void);

		/*!*********************************************************************************
		\brief
		Gets the Width of the Scene Panel

		\return
		Returns the selected Entity
		***********************************************************************************/
		f64 GetSceneWidth(void);

		/*!*********************************************************************************
		\brief
		Gets the Height of the Scene Panel

		\return
		Returns the selected Entity
		***********************************************************************************/
		f64 GetSceneHeight(void);

		/*!*********************************************************************************
		\brief
		Returns the Editor's Camera

		\return
		Gets the Editor's Camera
	***********************************************************************************/
		Engine::Camera& GetEditorCamera(void);

		/*!*********************************************************************************
			\brief
			Returns the cursor's world space position.

			\return
			Cursor World Space position.
			Returns a Vec2 containing std::numeric_limits::max for x and y if the mouse
			position was handled outside of the Scene viewport
		***********************************************************************************/
		Math::Vec2 GetMouseWorldPos();

		bool GetGameActive(void) { return m_GameStart; }

	private:
		/*!*********************************************************************************
			\brief
			Default constructor for the ALEditor class
		***********************************************************************************/
		ALEditor(void);

		/*!*********************************************************************************
			\brief
			Default destructor for the ALEditor class
		***********************************************************************************/
		~ALEditor(void) = default;

		/*!*********************************************************************************
			\brief
			Initialization for the ImGui editor
		***********************************************************************************/
		void Init(void);

		/*!*********************************************************************************
			\brief
			Runs code that enables docking for ImGui windows/panels.
			Is to be called every frame when Docking is enabled
		***********************************************************************************/
		void Docking(void);

		// Required for Singleton to function
		friend class Templates::Singleton<ALEditor>;
		friend class Memory::StaticMemory;

		// Window Min Size
		ImVec2 m_WinMinSize{ 320, 350 };

		// Variables
		b8 m_ImGuiEnabled{ false };		// Set to true if ImGui is enabled
		b8 m_DockingEnabled{ false };	// Set to true if docking is to be enabled
		b8 m_GameStart{ false };		// Set to true if in Game Mode

		// Panels
		ContentBrowserPanel m_ContentBrowserPanel;	// Content Browser Panel
		ScenePanel m_ScenePanel;					// Scene Panel (Editor)
		GamePanel m_GamePanel;						// Game Panel
		InspectorPanel m_InspectorPanel;			// Inspector Panel
		LoggerPanel m_LoggerPanel;					// Logger Panel
		//ProfilerPanel m_ProfilerPanel;				// Profiler Panel
		SceneHierarchyPanel m_SceneHierarchyPanel;	// Scene Hierarchy Panel
	};
}

#endif