#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		Matrix3x3::Matrix3x3(void) : matrix{ R, C } {}

		Matrix3x3::Matrix3x3(Vector3 f, Vector3 s, Vector3 t) : matrix{ R, C }
		{
			(*this)(0, 0) = f.x; (*this)(0, 1) = f.y; (*this)(0, 2) = f.z;
			(*this)(1, 0) = s.x; (*this)(1, 1) = s.y; (*this)(1, 2) = s.z;
			(*this)(2, 0) = t.x; (*this)(2, 1) = t.y; (*this)(2, 2) = t.z;
		}

		Matrix3x3::~Matrix3x3(void) {}

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

		Matrix3x3& Matrix3x3::Translate(float x, float y)
		{
			return *this = std::move(Mtx3x3Translate(x, y));
		}

		Matrix3x3& Matrix3x3::Translate(Vector2 const& rhs)
		{
			return *this = std::move(Mtx3x3Translate(rhs.x, rhs.y));
		}

		Matrix3x3& Matrix3x3::Scale(float x, float y)
		{
			return *this = std::move(Mtx3x3Scale(x, y));
		}

		Matrix3x3& Matrix3x3::Scale(Vector2 const& rhs)
		{
			return *this = std::move(Mtx3x3Scale(rhs.x, rhs.y));
		}

		Matrix3x3& Matrix3x3::RotationRad(float rad)
		{
			return *this = std::move(Mtx3x3RotRad(rad));
		}

		Matrix3x3& Matrix3x3::RotationDeg(float deg)
		{
			return *this = std::move(Mtx3x3RotDeg(deg));
		}

		Matrix3x3 Mtx3x3Translate(float x, float y)
		{
			Matrix3x3 mtx(Vector3(0.0f, 0.0f, x), Vector3(0.0f, 0.0f, y));
			return mtx;
		}

		Matrix3x3 Mtx3x3Translate(Vector2 const& rhs)
		{
			return Mtx3x3Translate(rhs.x, rhs.y);
		}

		Matrix3x3 Mtx3x3Scale(float x, float y)
		{
			Matrix3x3 mtx(Vector3(x, 0.0f, 0.0f), Vector3(0.0f, y, 0.0f));
			return mtx;
		}

		Matrix3x3 Mtx3x3Scale(Vector2 const& rhs)
		{
			return Mtx3x3Scale(rhs.x, rhs.y);
		}

		Matrix3x3 Mtx3x3RotRad(float rad)
		{
			using value_type = typename Matrix3x3::value_type;
			value_type const cos = static_cast<value_type>(std::cosf(rad)), sin = static_cast<value_type>(std::sinf(rad));
			Matrix3x3 mtx(Vector3(cos, -sin, 0.0f), Vector3(sin, cos, 0.0f));
			return mtx;
		}

		Matrix3x3 Mtx3x3RotDeg(float deg)
		{
			using value_type = typename Matrix3x3::value_type;
			value_type const rad = static_cast<value_type>(deg) * static_cast<value_type>(M_PI) / static_cast<value_type>(180.0);
			return Mtx3x3RotRad(rad);
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
			matrix tmp{ lhs }; tmp *= rhs;
			return Vector2(tmp(0, 0), tmp(1, 0));
		}
	}
}