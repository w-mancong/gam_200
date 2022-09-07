#include "pch.h"

namespace ALEngine
{
	namespace Math
	{
		Matrix3x3::Matrix3x3(value_type value) : matrix{ R, C }
		{
			for (u64 i = 0; i < R; ++i)
				(*this)(i, i) = value;
		}

		Matrix3x3::Matrix3x3(Vector3 r1, Vector3 r2, Vector3 r3) : matrix{ R, C }
		{
			(*this)(0, 0) = r1.x; (*this)(0, 1) = r1.y; (*this)(0, 2) = r1.z;
			(*this)(1, 0) = r2.x; (*this)(1, 1) = r2.y; (*this)(1, 2) = r2.z;
			(*this)(2, 0) = r3.x; (*this)(2, 1) = r3.y; (*this)(2, 2) = r3.z;
		}

		Matrix3x3& Matrix3x3::operator+=(Matrix3x3 const& rhs)
		{
			matrix::operator+=(rhs);
			return *this;
		}

		Matrix3x3& Matrix3x3::operator-=(Matrix3x3 const& rhs)
		{
			matrix::operator-=(rhs);
			return *this;
		}

		Matrix3x3& Matrix3x3::operator*=(Matrix3x3 const& rhs)
		{
			matrix::operator*=(rhs);
			return *this;
		}

		Matrix3x3& Matrix3x3::operator*=(value_type rhs)
		{
			matrix::operator*=(rhs);
			return *this;
		}

		Matrix3x3 Matrix3x3::Translate(f32 x, f32 y)
		{
			Matrix3x3 res(Vector3(0.0f, 0.0f, x), Vector3(0.0f, 0.0f, y));
			return res;
		}

		Matrix3x3 Matrix3x3::Translate(Vector2 const& rhs)
		{
			return Translate(rhs.x, rhs.y);
		}

		Matrix3x3 Matrix3x3::Scale(f32 x, f32 y)
		{
			Matrix3x3 res(Vector3(x, 0.0f, 0.0f), Vector3(0.0f, y, 0.0f));
			return res;
		}

		Matrix3x3 Matrix3x3::Scale(Vector2 const& rhs)
		{
			return Scale(rhs.x, rhs.y);
		}

		Matrix3x3 Matrix3x3::Rotation(f32 deg)
		{
			using value_type = typename Matrix3x3::value_type;
			// converting degree to rad
			value_type const rad = static_cast<value_type>(DegreeToRadian(deg));
			// calculation cos and sin
			value_type const cos = static_cast<value_type>(std::cosf(rad)), sin = static_cast<value_type>(std::sinf(rad));
			// setting matrix accordingly
			Matrix3x3 res(Vector3(cos, -sin, 0.0f), Vector3(sin, cos, 0.0f));
			return res;
		}

		Matrix3x3 operator+(Matrix3x3 const& lhs, Matrix3x3 const& rhs)
		{
			Matrix3x3 res{ lhs }; res += rhs;
			return res;
		}

		Matrix3x3 operator-(Matrix3x3 const& lhs, Matrix3x3 const& rhs)
		{
			Matrix3x3 res{ lhs }; res -= rhs;
			return res;
		}

		Matrix3x3 operator*(Matrix3x3 const& lhs, Matrix3x3 const& rhs)
		{
			Matrix3x3 res{ lhs }; res *= rhs;
			return res;
		}

		Matrix3x3 operator*(Matrix3x3 const& lhs, typename Matrix3x3::value_type rhs)
		{
			Matrix3x3 res{ lhs }; res *= rhs;
			return res;
		}

		Matrix3x3 operator*(typename Matrix3x3::value_type lhs, Matrix3x3 const& rhs)
		{
			Matrix3x3 res{ rhs }; res *= lhs;
			return res;
		}

		Vector2 operator*(Matrix3x3 const& lhs, Vector2 const& rhs)
		{
			matrix tmp{ lhs }; 			
			try
			{
				tmp *= rhs;
			}
			catch (std::exception const& e)
			{
				std::cerr << e.what() << std::endl;
				return Vector2();
			}
			return Vector2(tmp(0, 0), tmp(1, 0));
		}
	}
}