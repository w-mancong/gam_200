#include "pch.h"

namespace ALEngine
{
	namespace Math
	{
		Matrix4x4::Matrix4x4(value_type value) : matrix{ R, C }
		{
			for (u64 i = 0; i < R; ++i)
				(*this)(i, i) = value;
		}

		Matrix4x4::Matrix4x4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4) : matrix{ R, C }
		{
			(*this)(0, 0) = r1.x; (*this)(0, 1) = r1.y; (*this)(0, 2) = r1.z; (*this)(0, 3) = r1.w;
			(*this)(1, 0) = r2.x; (*this)(1, 1) = r2.y; (*this)(1, 2) = r2.z; (*this)(1, 3) = r2.w;
			(*this)(2, 0) = r3.x; (*this)(2, 1) = r3.y; (*this)(2, 2) = r3.z; (*this)(2, 3) = r3.w;
			(*this)(3, 0) = r4.x; (*this)(3, 1) = r4.y; (*this)(3, 2) = r4.z; (*this)(3, 3) = r4.w;
		}

		Matrix4x4& Matrix4x4::operator+=(Matrix4x4 const& rhs)
		{
			matrix::operator+=(rhs);
			return *this;
		}

		Matrix4x4& Matrix4x4::operator-=(Matrix4x4 const& rhs)
		{
			matrix::operator-=(rhs);
			return *this;
		}

		Matrix4x4& Matrix4x4::operator*=(Matrix4x4 const& rhs)
		{
			matrix::operator*=(rhs);
			return *this;
		}

		Matrix4x4& Matrix4x4::operator*=(value_type rhs)
		{
			matrix::operator*=(rhs);
			return *this;
		}

		Matrix4x4 Matrix4x4::Translate(f32 x, f32 y, f32 z)
		{
			Matrix4x4 res; res(3, 0) = x, res(3, 1) = y, res(3, 2) = z;
			return res;
		}

		Matrix4x4 Matrix4x4::Translate(Vector3 const& rhs)
		{
			return Translate(rhs.x, rhs.y, rhs.z);
		}

		Matrix4x4 Matrix4x4::Scale(f32 x, f32 y, f32 z)
		{
			Matrix4x4 res; res(0, 0) = x, res(1, 1) = y, res(2, 2) = z;
			return res;
		}

		Matrix4x4 Matrix4x4::Scale(Vector3 const& rhs)
		{
			return Scale(rhs.x, rhs.y, rhs.z);
		}

		Matrix4x4 Matrix4x4::Rotation(f32 angle, Vector3 const& axis)
		{
			Matrix4x4 res;
			f32 const rad = DegreeToRadian(angle);
			f32 const cos = std::cosf(rad), sin = std::sinf(rad), omc = 1.0f - cos;

			res(0, 0) = axis.x * axis.x * omc + cos;
			res(0, 1) = axis.x * axis.y * omc + axis.z * sin;
			res(0, 2) = axis.x * axis.z * omc - axis.y * sin;

			res(1, 0) = axis.x * axis.y * omc - axis.z * sin;
			res(1, 1) = axis.y * axis.y * omc + cos;
			res(1, 2) = axis.y * axis.z * omc + axis.x * sin;

			res(2, 0) = axis.x * axis.z * omc + axis.y * sin;
			res(2, 1) = axis.y * axis.z * omc - axis.x * sin;
			res(2, 2) = axis.z * axis.z * omc + cos;

			return res;
		}

		Matrix4x4 Matrix4x4::Ortho(f32 left, f32 right, f32 bottom, f32 top)
		{
			Matrix4x4 res{ 1.0f };
			res(0, 0) =  2.0f / (right - left);
			res(1, 1) =  2.0f / (top - bottom);
			res(2, 2) = -1.0f;
			res(3, 0) = -(right + left) / (right - left);
			res(3, 1) = -(top + bottom) / (top - bottom);
			return res;
		}

		Matrix4x4 Matrix4x4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
		{
			Matrix4x4 res{ 1.0f };
			res(0, 0) =  2.0f / (right - left);
			res(1, 1) =  2.0f / (top - bottom);
			res(2, 2) = -2.0f / (zFar - zNear);
			res(3, 0) = -(right + left) / (right - left);
			res(3, 1) = -(top + bottom) / (top - bottom);
			res(3, 2) = -(zFar + zNear) / (zFar - zNear);
			return res;
		}

		Matrix4x4 Matrix4x4::Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar)
		{
#if _DEBUG
			assert(abs(aspect - std::numeric_limits<f32>::epsilon()) > 0.0f);
#endif
			f32 const halfTan = std::tanf(DegreeToRadian(fov) / 2.0f);
			Matrix4x4 res{ 0.0f };
			res(0, 0) = 1.0f / (aspect * halfTan);
			res(1, 1) = 1.0f / halfTan;
			res(2, 2) = -(zFar + zNear) / (zFar - zNear);
			res(2, 3) = -1.0f;
			res(3, 2) = -(2.0f * zFar * zNear) / (zFar - zNear);
			return res;
		}

		Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 center, Vector3 up)
		{
			// direction										// right							   // up
			Vector3 const d = Vector3::Normalize(center - eye), r = Vector3::Cross(d, up).Normalize(), u = Vector3::Cross(r, d);
			Matrix4x4 res;
			res(0, 0) = r.x;				   res(0, 1) = u.x;					  res(0, 2) = -d.x;
			res(1, 0) = r.y;				   res(1, 1) = u.y;					  res(1, 2) = -d.y;
			res(2, 0) = r.z;				   res(2, 1) = u.z;					  res(2, 2) = -d.z;			
			res(3, 0) = -Vector3::Dot(r, eye); res(3, 1) = -Vector3::Dot(u, eye); res(3, 2) = Vector3::Dot(d, eye);		
			return res;
		}

		Matrix4x4 operator+(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
		{
			Matrix4x4 res{ lhs }; res += rhs;
			return res;
		}

		Matrix4x4 operator-(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
		{
			Matrix4x4 res{ lhs }; res -= rhs;
			return res;
		}

		Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
		{
			Matrix4x4 res{ lhs }; res *= rhs;
			return res;
		}

		Matrix4x4 operator*(Matrix4x4 const& lhs, typename Matrix4x4::value_type rhs)
		{
			Matrix4x4 res{ lhs }; res *= rhs;
			return res;
		}

		Matrix4x4 operator*(typename Matrix4x4::value_type lhs, Matrix4x4 const& rhs)
		{
			Matrix4x4 res{ rhs }; res *= lhs;
			return res;
		}

		Vector3 operator*(Matrix4x4 const& lhs, Vector3 const& rhs)
		{
			matrix tmp{ lhs }; 
			try
			{
				tmp *= rhs;
			}
			catch (std::exception const& e)
			{
				std::cerr << e.what() << std::endl;
			}
			return Vector2(tmp(0, 0), tmp(1, 0));
		}
	}
}