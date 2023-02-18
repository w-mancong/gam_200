/*!
file:	ScenePanel.h
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function declarations for the ScenePanel class.
		The ScenePanel class contains information and functions necessary for
		the Scene Panel of the editor to be displayed.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef SCENE_PANEL_H
#define SCENE_PANEL_H

#if EDITOR

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
			Returns the cursor's world space position.

			\return
			Cursor World Space position.
			Returns a Vec2 containing std::numeric_limits::max for x and y if the mouse
			position was handled outside of the Scene viewport
		***********************************************************************************/
		Math::Vec2 GetMouseWorldPos();

		/*!*********************************************************************************
			\brief
			Returns the cursor's screen space with respect to the Scene Panel.

			\return
			Cursor Screen Space position with respect to the Scene Panel.
		***********************************************************************************/
		Math::Vec2 GetMousePosWRTPanel();

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
			\brief Return editor panel camera's width
		***********************************************************************************/
		f32& GetCameraWidth(void);

		/*!*********************************************************************************
			\brief Return editor panel camera's height
		***********************************************************************************/
		f32& GetCameraHeight(void);

		/*!*********************************************************************************
			\brief
			Returns the Editor's Camera

			\return
			Gets the Editor's Camera
		***********************************************************************************/
		Engine::Camera& GetEditorCamera(void);

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
			\brief To control the camera movements
		***********************************************************************************/
		void UserInput(void);

		/*!*********************************************************************************
			\brief To control zoom in/out of camera
		***********************************************************************************/
		void CameraZoom(void);

		// Panel sizes
		ImVec2 m_PanelMin{};	// Min Size

		// Panel Defaults
		ImVec2 m_DefaultPos{};	// Default Position
		ImVec2 m_DefaultSize{};	// Default Size

		// Scene Size
		f32 m_SceneWidth{ 0 }, m_SceneHeight{ 0 };		

		ImVec2 m_ImGuiMousePos{};
		ImVec2 m_ImGuiPanelPos{};

		// Camera
		Engine::Camera m_EditorCamera{ Math::Vec3(0.f, 0.f, 725.f) };
		f32 m_CameraWidth{ 0.0f }, m_CameraHeight{ 0.0f };
		static f32 constexpr const ZOOM_SPEED{ 50.0f };
	};

	/*!*********************************************************************************
		\brief
		Checks if the point and object are colliding
		\param [in] pointPos
		Position of point
		\param [in] aabbPos
		Position of AABB's center
		\param [in] width
		AABB width
		\param [in] height
		AABB height
		\Brief
		Returns true is colliding, else returns false
	***********************************************************************************/
	bool Check_Point_To_AABB(Math::Vec2 pointPos, Math::Vec2 aabbPos, float width, float height);
}
#endif

#endif