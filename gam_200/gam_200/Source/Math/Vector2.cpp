#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		Vector2 const Vector2::right			= Vector2(1.0f, 0.0f);
		Vector2 const Vector2::left				= Vector2(-1.0f, 0.0f);
		Vector2 const Vector2::up				= Vector2(0.0f, 1.0f);
		Vector2 const Vector2::down				= Vector2(0.0f, -1.0f);
		Vector2 const Vector2::one				= Vector2(1.0f, 1.0f);
		Vector2 const Vector2::zero				= Vector2(0.0f, 0.0f);
		Vector2 const Vector2::positiveInfinity = Vector2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
		Vector2 const Vector2::negativeInfinity = Vector2(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

		Vector2::Vector2(void) : x{ 0.0f }, y{ 0.0f } {}
		Vector2::Vector2(float x, float y) : x{ x }, y{ y } {}
		Vector2::Vector2(Vector3 const& rhs) : x{ rhs.x }, y{ rhs.y } {}
		Vector2::~Vector2(void) {}
		
		Vector2& Vector2::operator=(Vector3 const& rhs)
		{
			Vector2 tmp{ rhs };
			std::swap(*this, tmp);
			return *this;
		}

		/*********************************************************************************
									MATH ARITHMETIC
		*********************************************************************************/
		Vector2& Vector2::operator+=(Vector2 const& rhs)
		{
			x += rhs.x, y += rhs.y;
			return *this;
		}

		Vector2& Vector2::operator-=(Vector2 const& rhs)
		{
			x -= rhs.x, y -= rhs.y;
			return *this;
		}

		Vector2& Vector2::operator*=(float rhs)
		{
			x *= rhs, y *= rhs;
			return *this;
		}

		Vector2& Vector2::operator/=(float rhs)
		{
			x /= rhs, y /= rhs;
			return *this;
		}

		Vector2 Vector2::operator-(void) const
		{
			return Vector2(-x, -y);
		}

		float Vector2::Dot(Vector2 const& rhs) const
		{
			return x * rhs.x + y * rhs.y;
		}

		float Vector2::Magnitude(void) const
		{
			return sqrtf(MagnitudeSq());
		}

		float Vector2::MagnitudeSq(void) const
		{
			return x * x + y * y;
		}

		void Vector2::Normalized(void)
		{
			float const inv_len = Q_rsqrt(MagnitudeSq());
			x *= inv_len; y *= inv_len;
		}

		Vector2 Vector2::Normalize(void) const
		{
			Vector2 res{ *this };
			res.Normalized();
			return res;
		}

		/*********************************************************************************
											   UTILITIES
		*********************************************************************************/
		bool Vector2::operator==(Vector2 const& rhs) const
		{
			return Utility::IsEqual(x, rhs.x) && Utility::IsEqual(y, rhs.y);
		}

		bool Vector2::operator!=(Vector2 const& rhs) const
		{
			return !(*this == rhs);
		}

		void Vector2::SetZero(void)
		{
			x = 0.0f, y = 0.0f;
		}

		bool Vector2::IsZero(void) const
		{
			return Utility::IsEqual(x, 0.0f) && Utility::IsEqual(y, 0.0f);
		}

		float Vector2::Angle(Vector2 const& from, Vector2 const& to)
		{
			return std::acosf(Vector2::Dot(from, to) / (from.Magnitude() * to.Magnitude())) * 180.0f / static_cast<float>(M_PI);
		}

		Vector2 Vector2::ClampMagnitude(Vector2 const& lhs, float maxLength)
		{
			Vector2 tmp{ lhs };
			return tmp.Normalize() * maxLength;
		}

		float Vector2::Distance(Vector2 const& lhs, Vector2 const& rhs)
		{
			Vector2 tmp{ lhs - rhs };
			return tmp.Magnitude();
		}

		float Vector2::DistanceSq(Vector2 const& lhs, Vector2 const& rhs)
		{
			Vector2 tmp{ lhs - rhs };
			return tmp.MagnitudeSq();
		}

		float Vector2::Dot(Vector2 const& lhs, Vector2 const& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}

		Vector2 Vector2::Max(Vector2 const& lhs, Vector2 const& rhs)
		{
			return Vector2(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
		}

		Vector2 Vector2::Min(Vector2 const& lhs, Vector2 const& rhs)
		{
			return Vector2(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
		}

		Vector2 Vector2::Perpendicular(Vector2 const& inDirection)
		{
			return Vector2(-inDirection.y, inDirection.x);
		}

		//Vector2 Vector2::Reflect(Vector2 const& inDirection, Vector2 const& inNormal)
		//{
		//}

		Vector2 Vector2::Normalize(Vector2 const& rhs)
		{
			Vector2 tmp{ rhs };
			return tmp.Normalize();
		}

		Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs)
		{
			Vector2 res{ lhs }; res += rhs;
			return res;
		}

		Vector2 operator-(Vector2 const& lhs, Vector2 const& rhs)
		{
			Vector2 res{ lhs }; res -= rhs;
			return res;
		}

		Vector2 operator*(Vector2 const& lhs, float rhs)
		{
			Vector2 res{ lhs }; res *= rhs;
			return res;
		}

		Vector2 operator*(float lhs, Vector2 const& rhs)
		{
			Vector2 res{ rhs }; res *= lhs;
			return res;
		}

		Vector2 operator/(Vector2 const& lhs, float rhs)
		{
			Vector2 res{ lhs }; res /= rhs;
			return res;
		}

		std::ostream& operator<<(std::ostream& os, Vector2 const& rhs)
		{
			return os << "x: " << rhs.x << " y: " << rhs.y;
		}
	}
}