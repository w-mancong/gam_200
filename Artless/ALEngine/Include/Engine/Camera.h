/*!
file:	Camera.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contains class declaration for camera

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	CAMERA_H
#define CAMERA_H

namespace ALEngine::Engine
{
	using namespace Math;
	class Camera
	{
	public:
		enum class Projection
		{
			Perspective,
			Orthographic
		};
	public:
		Camera(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f), f32 yaw = -90.0f, f32 pitch = 0.0f, f32 zNear = 0.1f, f32 zFar = 1000.0f, f32 fov = 45.0f);
		Camera(f32 x_pos = 0.0f, f32 y_pos = 0.0f, f32 z_pos = 0.0f, f32 x_up = 0.0f, f32 y_up = 1.0f, f32 z_up = 0.0f, f32 yaw = -90.0f, f32 pitch = 0.0f, f32 zNear = 0.1f, f32 zFar = 100.0f, f32 fov = 45.0f);

		/*!*********************************************************************************
			\brief Update function for camera
		***********************************************************************************/
		void Update(void);

		/*!*********************************************************************************
			\brief Return angle for yaw
		***********************************************************************************/
		f32 Yaw(void) const;

		/*!*********************************************************************************
			\brief
			Return angle for pitch
		***********************************************************************************/
		f32 Pitch(void) const;

		/*!*********************************************************************************
			\brief
			Return the distance of near plane
		***********************************************************************************/
		f32 NearPlane(void) const;

		/*!*********************************************************************************
			\brief
			Return the distance of far plane
		***********************************************************************************/
		f32 FarPlane(void) const;

		/*!*********************************************************************************
			\brief
			Return the field of view of the camera
		***********************************************************************************/
		f32 Fov(void) const;

		/*!*********************************************************************************
			\brief
			Return the up vector of camera
		***********************************************************************************/
		Vector3 Up(void) const;

		/*!*********************************************************************************
			\brief
			Return the right vector of camera
		***********************************************************************************/
		Vector3 Right(void) const;

		/*!*********************************************************************************
			\brief Return the front vector of camera (where the camera is facing)
		***********************************************************************************/
		Vector3 Front(void) const;

		/*!*********************************************************************************
			\brief Return a reference to bottom coordinates for creating orthographic projection
		***********************************************************************************/
		f32& ProjBottom(void);

		/*!*********************************************************************************
			\brief Return a reference to top coordinates for creating orthographic projection
		***********************************************************************************/
		f32& ProjTop(void);

		/*!*********************************************************************************
			\brief Return a reference to left coordinates for creating orthographic projection
		***********************************************************************************/
		f32& ProjLeft(void);

		/*!*********************************************************************************
			\brief Return a reference to right coordinates for creating orthographic projection
		***********************************************************************************/
		f32& ProjRight(void);

		/*!*********************************************************************************
			\brief Return a const reference to bottom coordinates for creating orthographic projection
		***********************************************************************************/
		f32 const& ProjBottom(void) const;

		/*!*********************************************************************************
			\brief Return a const reference to top coordinates for creating orthographic projection
		***********************************************************************************/
		f32 const& ProjTop(void) const;

		/*!*********************************************************************************
			\brief Return a const reference to left coordinates for creating orthographic projection
		***********************************************************************************/
		f32 const& ProjLeft(void) const;

		/*!*********************************************************************************
			\brief Return a const reference to right coordinates for creating orthographic projection
		***********************************************************************************/
		f32 const& ProjRight(void) const;

		/*!*********************************************************************************
			\brief
			Return position of camera
		***********************************************************************************/
		Vector3 Position(void) const;

		/*!*********************************************************************************
			\brief
			Calculate and return view matrix
		***********************************************************************************/
		Matrix4x4 ViewMatrix(void) const;

		/*!*********************************************************************************
			\brief
			Calculate and return projection matrix
		***********************************************************************************/
		Matrix4x4 ProjectionMatrix(void) const;

		/*!*********************************************************************************
			\brief Calculate and return a perspective matrix
		***********************************************************************************/
		Matrix4x4 PerspectiveMatrix(void) const;
		
		/*!*********************************************************************************
			\brief Calcualte and return an orthographic matrix
		***********************************************************************************/
		Matrix4x4 OrthographicMatrix(void) const;

		/*!*********************************************************************************
			\brief Calcualte and return an orthographic matrix

			\param [in] l: Left corner of box
			\param [in] r: Right corner of box
			\param [in] b: Bottom corner of box
			\param [in] t: Top corner of box
		***********************************************************************************/
		Matrix4x4 OrthographicMatrix(f32 l, f32 r, f32 b, f32 t) const;

		/*!*********************************************************************************
			\brief Calcualte and return an orthographic matrix

			\param [in] l: Left corner of box
			\param [in] r: Right corner of box
			\param [in] b: Bottom corner of box
			\param [in] t: Top corner of box
			\param [in] n: Near corner of box
			\param [in] f: Far corner of box
		***********************************************************************************/
		Matrix4x4 OrthographicMatrix(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) const;

		/*!*********************************************************************************
			\brief Calcualte and return an orthographic matrix for imgui
		***********************************************************************************/
		Matrix4x4 OrthographicMatrixImgui(void) const;

		/*!*********************************************************************************
			\brief Return a reference to camera's position
		***********************************************************************************/
		vec3& Position(void);

		/*!*********************************************************************************
			\brief
			Set camera position

			\param [in] x:
			New x position for camera
			\param [in] y:
			New y position for camera
		***********************************************************************************/
		void Position(f32 x, f32 y);

		/*!*********************************************************************************
			\brief
			Set camera position

			\param [in] x:
			New x position for camera
			\param [in] y:
			New y position for camera
			\param [in] z:
			New z position for camera
		***********************************************************************************/
		void Position(f32 x, f32 y, f32 z);

		/*!*********************************************************************************
			\brief
			Set camera position

			\param [in] pos:
			New position for camera
		***********************************************************************************/
		void Position(Vector3 pos);

		/*!*********************************************************************************
			\brief
			Set the type of projection that the camera will be using

			\param [in] proj:
			Type of projection that the camera will be using
		***********************************************************************************/
		// -------------------------------------------
		void ProjectionMatrix(Projection proj);

		/*!*********************************************************************************
			\brief
			Set the near plane of the camera

			\param [in] zNear:
			New near plane for the camera
		***********************************************************************************/
		void NearPlane(f32 zNear);

		/*!*********************************************************************************
			\brief
			Set the far plane of the camera

			\param [in] zFar:
			New far plane for the camera
		***********************************************************************************/
		void FarPlane(f32 zFar);

		/*!*********************************************************************************
			\brief
			Set the fov of the camera

			\param [in] fov:
			New fov angle for the camera
		***********************************************************************************/
		void Fov(f32 fov);

		/*!*********************************************************************************
			\brief
			Set the rotation of camera, thus changing where the camera will be looking

			\param [in] yaw:
			Rotation of camera about the y-axis
			\param [in] pitch:
			Rotation of camera about the x-axis
		***********************************************************************************/
		void Rotation(f32 yaw, f32 pitch);

		/*!*********************************************************************************
			\brief
			Set the rotation of camera, thus changing where the camera will be looking
				
			\param [in] rotation:
			Contains the rotation of camera about the x and y-axis
		***********************************************************************************/
		void Rotation(Vector2 rotation);

	private:

		/*!*********************************************************************************
			\brief
			Helper function to update the appropriate vectors after changing the rotation of camera
		***********************************************************************************/
		void UpdateVectors(void);

		f32 m_Yaw{}, m_Pitch{}, m_Near{}, m_Far{}, m_Fov{};
		f32 m_ProjBottom{}, m_ProjTop{}, m_ProjLeft{}, m_ProjRight{};
		Vector3 m_Position{}, m_Right{}, m_Up{}, m_Front{}, m_WorldUp{};
		Projection m_Projection{};
	};
}

#endif