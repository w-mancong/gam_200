#include "pch.h"

namespace ALEngine::Engine
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

	f32 Camera::NearPlane(void) const
	{
		return m_Near;
	}

	f32 Camera::FarPlane(void) const
	{
		return m_Far;
	}

	f32 Camera::Fov(void) const
	{
		return m_Fov;
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

	Vector3 Camera::Position(void) const
	{
		return m_Position;
	}

	Matrix4x4 Camera::ViewMatrix(void) const
	{
		return Matrix4x4::LookAt(m_Position, m_Position + m_Front, m_Up);
	}

	Matrix4x4 Camera::ProjectionMatrix(void) const
	{
		switch (m_Projection)
		{
			case Projection::Perspective:
				return PerspectiveMatrix();
			case Projection::Orthographic:
				return OrthographicMatrix();
		}
		return Matrix4x4{ 1.0f };
	}

	Matrix4x4 Camera::PerspectiveMatrix(void) const
	{
		using namespace Graphics;
		return Matrix4x4::Perspective(m_Fov, static_cast<f32>(OpenGLWindow::width) / static_cast<f32>(OpenGLWindow::height), m_Near, m_Far);
	}

	Matrix4x4 Camera::OrthographicMatrix(void) const
	{
		using namespace Graphics;
		return Matrix4x4::Ortho(0.0f, static_cast<f32>(OpenGLWindow::width), 0.0f, static_cast<f32>(OpenGLWindow::height), m_Near, m_Far);
	}

	Matrix4x4 Camera::OrthographicMatrix(f32 l, f32 r, f32 b, f32 t) const
	{
		using namespace Graphics;
		return Matrix4x4::Ortho(l, r, b, t, m_Near, m_Far);
	}

	Matrix4x4 Camera::OrthographicMatrix(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) const
	{
		using namespace Graphics;
		return Matrix4x4::Ortho(l, r, b, t, n, f);
	}

	Matrix4x4 Camera::OrthographicMatrixImgui(void) const
	{
		using namespace Graphics;
		return Matrix4x4::OrthoImgui(0.0f, static_cast<f32>(OpenGLWindow::width), 0.0f, static_cast<f32>(OpenGLWindow::height), m_Near, m_Far);
	}

	void Camera::Position(f32 x, f32 y)
	{
		Position(Vector3(x, y, m_Position.z));
	}

	void Camera::Position(f32 x, f32 y, f32 z)
	{
		Position(Vector3(x, y, z));
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

	void Camera::Fov(f32 fov)
	{
		m_Fov = fov;
	}

	void Camera::Rotation(f32 yaw, f32 pitch)
	{
		Rotation(Vector2(pitch, yaw));
	}

	void Camera::Rotation(Vector2 rotation)
	{
		m_Pitch = rotation.x, m_Yaw = rotation.y;
		m_Pitch = Math::Clamp(m_Pitch, -89.0f, 89.0f);
		UpdateVectors();
	}

	void Camera::UpdateVectors(void)
	{
		f32 const yaw_sin	= std::sin(DegreeToRadian(m_Yaw)),	 yaw_cos	= std::cos(DegreeToRadian(m_Yaw));
		f32 const pitch_sin = std::sin(DegreeToRadian(m_Pitch)), pitch_cos	= std::cos(DegreeToRadian(m_Pitch));
		m_Front = Vector3(yaw_cos * pitch_cos, pitch_sin, yaw_sin * pitch_cos).Normalize();
		m_Right = Vector3::Cross(m_Front, m_WorldUp).Normalize();
		m_Up	= Vector3::Cross(m_Right, m_Front).Normalize();
	}
}