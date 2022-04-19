#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		Vector3 const Vector3::right			= Vector3(1.0f, 0.0f, 0.0f);
		Vector3 const Vector3::left				= Vector3(-1.0f, 0.0f, 0.0f);
		Vector3 const Vector3::up				= Vector3(0.0f, 1.0f, 0.0f);
		Vector3 const Vector3::down				= Vector3(0.0f, -1.0f, 0.0f);
		Vector3 const Vector3::forward			= Vector3(0.0f, 0.0f, 1.0f);
		Vector3 const Vector3::back				= Vector3(0.0f, 0.0f, -1.0f);
		Vector3 const Vector3::one				= Vector3(1.0f, 1.0f, 1.0f);
		Vector3 const Vector3::zero				= Vector3(0.0f, 0.0f, 0.0f);
		Vector3 const Vector3::positiveInfinity = Vector3(std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
		Vector3 const Vector3::negativeInfinity = Vector3(-std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());

		Vector3::Vector3(void) : x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}
		Vector3::Vector3(f32 x, f32 y, f32 z) : x{ x }, y{ y }, z{ z } {}
		Vector3::Vector3(Vector2 const& rhs) : x{ rhs.x }, y{ rhs.y }, z{ 0.0f } {}
		Vector3::Vector3(Vector4 const& rhs) : x{ rhs.x }, y{ rhs.y }, z{ rhs.z } {}

		Vector3& Vector3::operator=(Vector2 const& rhs)
		{
			Vector3 tmp{ rhs };
			std::swap(*this, tmp);
			return *this;
		}

		Vector3& Vector3::operator=(Vector4 const& rhs)
		{
			Vector3 tmp{ rhs };
			swap(tmp);
			return *this;
		}

		/*********************************************************************************
									MATH ARITHMETIC
		*********************************************************************************/
		Vector3& Vector3::operator+=(Vector3 const& rhs)
		{
			x += rhs.x, y += rhs.y, z += rhs.z;
			return *this;
		}

		Vector3& Vector3::operator-=(Vector3 const& rhs)
		{
			x -= rhs.x, y -= rhs.y, z -= rhs.z;
			return *this;
		}

		Vector3& Vector3::operator*=(f32 rhs)
		{
			x *= rhs, y *= rhs, z *= rhs;
			return *this;
		}

		Vector3& Vector3::operator/=(f32 rhs)
		{
			x /= rhs, y /= rhs, z / rhs;
			return *this;
		}

		Vector3 Vector3::operator-(void) const
		{
			return Vector3(-x, -y, -z);
		}

		f32 Vector3::Dot(Vector3 const& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		f32 Vector3::Length(void) const
		{
			return sqrtf(LengthSq());
		}

		f32 Vector3::LengthSq(void) const
		{
			return x * x + y * y + z * z;
		}

		void Vector3::Normalized(void)
		{
			f32 const inv_len = Q_rsqrt(LengthSq());
			x *= inv_len; y *= inv_len, z *= inv_len;
		}

		Vector3 Vector3::Normalize(void) const
		{
			Vector3 res{ *this };
			res.Normalized();
			return res;
		}

		/*********************************************************************************
											   UTILITIES
		*********************************************************************************/
		bool Vector3::operator==(Vector3 const& rhs) const
		{
			return Utility::IsEqual(x, rhs.x) && Utility::IsEqual(y, rhs.y) && Utility::IsEqual(z, rhs.z);
		}

		bool Vector3::operator!=(Vector3 const& rhs) const
		{
			return !(*this == rhs);
		}

		void Vector3::SetZero(void)
		{
			x = 0.0f, y = 0.0f, z = 0.0f;
		}

		bool Vector3::IsZero(void) const
		{
			return Utility::IsEqual(x, 0.0f) && Utility::IsEqual(y, 0.0f) && Utility::IsEqual(z, 0.0f);
		}

		void Vector3::swap(Vector3& rhs)
		{
			std::swap(x, rhs.x);
			std::swap(y, rhs.y);
			std::swap(z, rhs.z);
		}

		Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs)
		{
			Vector3 res{ lhs };
			res += rhs;
			return res;
		}

		Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs)
		{
			Vector3 res{ lhs };
			res -= rhs;
			return res;
		}

		Vector3 operator*(Vector3 const& lhs, f32 rhs)
		{
			Vector3 res{ lhs };
			res *= rhs;
			return res;
		}

		Vector3 operator*(f32 lhs, Vector3 const& rhs)
		{
			Vector3 res{ rhs };
			res *= lhs;
			return res;
		}

		Vector3 operator/(Vector3 const& lhs, f32 rhs)
		{
			Vector3 res{ lhs };
			res /= rhs;
			return res;
		}

		f32 Vector3Distance(Vector3 const& lhs, Vector3 const& rhs)
		{
			return sqrtf(Vector3SquareDistance(lhs, rhs));
		}

		f32 Vector3SquareDistance(Vector3 const& lhs, Vector3 const& rhs)
		{
			Vector3 res{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
			return res.LengthSq();
		}

		std::ostream& operator<<(std::ostream& os, Vector3 const& rhs)
		{
			return os << "x: " << rhs.x << " y: " << rhs.y << " z: " << rhs.z;
		}
	}
}