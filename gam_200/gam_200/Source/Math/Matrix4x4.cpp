#include "pch.h"

namespace ManCong
{
	namespace Math
	{
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
			Matrix4x4 res(Vector4(1.0f, 0.0f, 0.0f, x), Vector4(0.0f, 1.0f, 0.0f, y), Vector4(0.0f, 0.0f, 1.0f, z));
			res.Transpose();
			return res;
		}

		Matrix4x4 Matrix4x4::Translate(Vector3 const& rhs)
		{
			return Translate(rhs.x, rhs.y, rhs.z);
		}

		Matrix4x4 Matrix4x4::Scale(f32 x, f32 y, f32 z)
		{
			Matrix4x4 res(Vector4(x, 0.0f, 0.0f, 0.0f), Vector4(0.0f, y, 0.0f, 0.0f), Vector4(0.0f, 0.0f, z, 0.0f));
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