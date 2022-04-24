#ifndef	CAMERA_H
#define CAMERA_H

#include "Utility/Type.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
namespace ManCong
{
	namespace Engine
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
			Camera(Vector3 pos = Vector3::zero, Vector3 up = Vector3::up, f32 yaw = -90.0f, f32 pitch = 0.0f, f32 zNear = 0.1f, f32 zFar = 100.0f, f32 fov = 45.0f);
			Camera(f32 x_pos = 0.0f, f32 y_pos = 0.0f, f32 z_pos = 0.0f, f32 x_up = 0.0f, f32 y_up = 1.0f, f32 z_up = 0.0f, f32 yaw = -90.0f, f32 pitch = 0.0f, f32 zNear = 0.1f, f32 zFar = 100.0f, f32 fov = 45.0f);

			f32 Yaw(void) const;
			f32 Pitch(void) const;
			Vector3 Up(void) const;
			Vector3 Right(void) const;
			Vector3 Front(void) const;
			Vector3 Position(void) const;
			Matrix4x4 ViewMatrix(void) const;
			Matrix4x4 ProjectionMatrix(void) const;

			void Position(f32 x, f32 y);
			void Position(f32 x, f32 y, f32 z);
			void Position(Vector3 pos);
			// -------------------------------------------
			void ProjectionMatrix(Projection proj);
			// -------------------------------------------
			void NearPlane(f32 zNear);
			void FarPlane(f32 zFar);
			// -------------------------------------------
			void Rotation(f32 yaw, f32 pitch);
			void Rotation(Vector2 rotation);

		private:
			void UpdateVectors(void);

			f32 m_Yaw, m_Pitch, m_Near, m_Far, m_Fov;
			Vector3 m_Position, m_Right, m_Up, m_Front, m_WorldUp;
			Projection m_Projection;
		};
	}
}

#endif