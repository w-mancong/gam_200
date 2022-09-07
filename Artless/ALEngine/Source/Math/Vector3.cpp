#include "pch.h"

namespace ALEngine
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

		f32 Vector3::Magnitude(void) const
		{
			return sqrtf(MagnitudeSq());
		}

		f32 Vector3::MagnitudeSq(void) const
		{
			return Dot(*this);
		}

		void Vector3::Normalized(void)
		{
			f32 const inv_len = Q_rsqrt(MagnitudeSq());
			x *= inv_len; y *= inv_len, z *= inv_len;

			//f32 const length = Magnitude();
			//x /= length, y /= length, z /= length;
		}

		Vector3 Vector3::Normalize(void) const
		{
			Vector3 res{ *this };
			res.Normalized();
			return res;
		}

		Vector3 Vector3::Cross(Vector3 const& rhs) const
		{
			Vector3 res;
			res.x = y * rhs.z - z * rhs.y;
			res.y = z * rhs.x - x * rhs.z;
			res.z = x * rhs.y - y * rhs.x;
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

		f32 Vector3::Angle(Vector3 const& from, Vector3 const& to)
		{
			return RadianToDegree( std::acosf( Vector3::Dot(from, to) / ( from.Magnitude() * to.Magnitude() ) ) );
		}

		Vector3 Vector3::ClampMagnitude(Vector3 const& lhs, f32 maxLength)
		{
			Vector3 tmp{ lhs };
			return tmp.Normalize() * maxLength;
		}

		f32 Vector3::Distance(Vector3 const& lhs, Vector3 const& rhs)
		{
			Vector3 tmp{ lhs - rhs };
			return tmp.Magnitude();
		}

		f32 Vector3::DistanceSq(Vector3 const& lhs, Vector3 const& rhs)
		{
			Vector3 tmp{ lhs - rhs };
			return tmp.MagnitudeSq();
		}

		f32 Vector3::Dot(Vector3 const& lhs, Vector3 const& rhs)
		{
			return lhs.Dot(rhs);
		}

		Vector3 Vector3::Max(Vector3 const& lhs, Vector3 const& rhs)
		{
			return Vector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
		}

		Vector3 Vector3::Min(Vector3 const& lhs, Vector3 const& rhs)
		{
			return Vector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
		}

		Vector3 Vector3::Normalize(Vector3 const& rhs)
		{
			Vector3 tmp{ rhs };
			return tmp.Normalize();
		}

		Vector3 Vector3::Cross(Vector3 const& lhs, Vector3 const& rhs)
		{
			return lhs.Cross(rhs);
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

		std::ostream& operator<<(std::ostream& os, Vector3 const& rhs)
		{
			return os << std::fixed << std::setprecision(5) << std::left 
				<< "x: " << std::setw(10) << rhs.x 
				<< "y: " << std::setw(10) << rhs.y 
				<< "z: " << std::setw(10) << rhs.z;
		}
	}
}