#include "pch.h"
#include "Engine/Camera.h"

namespace ManCong
{
	namespace Engine
	{
		Camera::Camera(Vector3 pos, Vector3 up, f32 yaw, f32 pitch, f32 zNear, f32 zFar, f32 fov) : m_Yaw{ yaw }, m_Pitch{ pitch }, m_Near{ zNear }, m_Far{ zFar }, m_Fov{ fov }, m_Position{ pos }, m_Up{ up }, m_WorldUp{ m_Up }, m_Projection{ Projection::Perspective }
		{
			UpdateVectors();
		}

		Camera::Camera(f32 x_pos, f32 y_pos, f32 z_pos, f32 x_up, f32 y_up, f32 z_up, f32 yaw, f32 pitch, f32 zNear, f32 zFar, f32 fov) : m_Yaw{ yaw }, m_Pitch{ pitch }, m_Near{ zNear }, m_Far{ zFar }, m_Fov{ fov }, m_Position{ x_pos, y_pos, z_pos }, m_Up{ x_up, y_up, z_up }, m_WorldUp{ m_Up }, m_Projection{ Projection::Perspective }
		{
			UpdateVectors();
		}

		f32 Camera::Yaw(void) const
		{
			return m_Yaw;
		}

		f32 Camera::Pitch(void) const
		{
			return m_Pitch;
		}

		Vector3 Camera::Up(void) const
		{
			return m_Up;
		}

		Vector3 Camera::Right(void) const
		{
			return m_Right;
		}

		Vector3 Camera::Front(void) const
		{
			return m_Front;
		}

		Matrix4x4 Camera::ViewMatrix(void) const
		{
			return Matrix4x4::LookAt(m_Position, m_Position + m_Front, m_Up);
		}

		Matrix4x4 Camera::ProjectionMatrix(void) const
		{
			using namespace Graphics;
			switch (m_Projection)
			{
				case Projection::Perspective:
					return Matrix4x4::Perspective(m_Fov, static_cast<f32>(*OpenGLWindow::WindowWidth) / static_cast<f32>(*OpenGLWindow::WindowHeight), m_Near, m_Far);
				case Projection::Orthographic:
					return Matrix4x4::Ortho(0.0f, static_cast<f32>(*OpenGLWindow::WindowWidth), 0.0f, static_cast<f32>(*OpenGLWindow::WindowHeight), m_Near, m_Far);
			}
		}

		void Camera::Position(f32 x, f32 y)
		{
			m_Position.x = x, m_Position.y = y;
		}

		void Camera::Position(f32 x, f32 y, f32 z)
		{
			m_Position.x = x, m_Position.y = y, m_Position.z = z;
		}

		void Camera::Position(Vector3 pos)
		{
			m_Position = pos;
		}

		void Camera::ProjectionMatrix(Projection proj)
		{
			m_Projection = proj;
		}

		void Camera::NearPlane(f32 zNear)
		{
			m_Near = zNear;
		}

		void Camera::FarPlane(f32 zFar)
		{
			m_Far = zFar;
		}

		void Camera::Rotation(f32 yaw, f32 pitch)
		{
			Rotation(Vector2(yaw, pitch));
		}

		void Camera::Rotation(Vector2 rotation)
		{
			m_Yaw = rotation.x, m_Pitch = rotation.y;
			m_Pitch = Math::Clamp(m_Pitch, -89.0f, 89.0f);
			UpdateVectors();
		}

		void Camera::UpdateVectors(void)
		{
			f32 const yaw_sin = std::sinf(DegreeToRadian(m_Yaw)), yaw_cos = std::cosf(DegreeToRadian(m_Yaw));
			f32 const pitch_sin = std::sinf(DegreeToRadian(m_Pitch)), pitch_cos = std::cosf(DegreeToRadian(m_Pitch));
			m_Front = Vector3(yaw_cos * pitch_cos, pitch_sin, yaw_sin * pitch_cos).Normalize();
			m_Right = Vector3::Cross(m_Front, m_WorldUp).Normalize();
			m_Up	= Vector3::Cross(m_Right, m_Front).Normalize();
		}
	}
}