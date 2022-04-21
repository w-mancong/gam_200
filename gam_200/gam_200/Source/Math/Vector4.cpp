#include "pch.h"

namespace ManCong
{
	namespace Math
	{
		Vector4::Vector4(void) : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f } {}
		Vector4::Vector4(f32 x, f32 y, f32 z, f32 w) : x{ x }, y{ y }, z{ z }, w{ w } {}
		Vector4::Vector4(Vector2 const& rhs) : x{ rhs.x }, y{ rhs.y }, z{ 0.0f }, w{ 1.0f } {}
		Vector4::Vector4(Vector3 const& rhs) : x{ rhs.x }, y{ rhs.y }, z{ rhs.z }, w{ 1.0f } {}

		Vector4& Vector4::operator=(Vector2 const& rhs)
		{
			Vector4 tmp{ rhs };
			std::swap(*this, tmp);
			return *this;
		}

		Vector4& Vector4::operator=(Vector3 const& rhs)
		{
			Vector4 tmp{ rhs };
			swap(tmp);
			return *this;
		}

		/*********************************************************************************
									MATH ARITHMETIC
		*********************************************************************************/
		Vector4& Vector4::operator+=(Vector4 const& rhs)
		{
			x += rhs.x, y += rhs.y, z += rhs.z, w += rhs.w;
			return *this;
		}

		Vector4& Vector4::operator-=(Vector4 const& rhs)
		{
			x -= rhs.x, y -= rhs.y, z -= rhs.z, w -= rhs.w;
			return *this;
		}

		Vector4& Vector4::operator*=(f32 rhs)
		{
			x *= rhs, y *= rhs, z *= rhs, w *= rhs;
			return *this;
		}

		Vector4& Vector4::operator/=(f32 rhs)
		{
			x /= rhs, y /= rhs, z / rhs, w / rhs;
			return *this;
		}

		Vector4 Vector4::operator-(void) const
		{
			return Vector4(-x, -y, -z, -w);
		}

		f32 Vector4::Dot(Vector4 const& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}

		f32 Vector4::Length(void) const
		{
			return sqrtf(LengthSq());
		}

		f32 Vector4::LengthSq(void) const
		{
			return x * x + y * y + z * z + w * w;
		}

		void Vector4::Normalized(void)
		{
			f32 const inv_len = Q_rsqrt(LengthSq());
			x *= inv_len; y *= inv_len, z *= inv_len, w *= inv_len;
		}

		Vector4 Vector4::Normalize(void) const
		{
			Vector4 res{ *this };
			res.Normalized();
			return res;
		}

		/*********************************************************************************
											   UTILITIES
		*********************************************************************************/
		bool Vector4::operator==(Vector4 const& rhs) const
		{
			return Utility::IsEqual(x, rhs.x) && Utility::IsEqual(y, rhs.y) && Utility::IsEqual(z, rhs.z) && Utility::IsEqual(w, rhs.w);
		}

		bool Vector4::operator!=(Vector4 const& rhs) const
		{
			return !(*this == rhs);
		}

		void Vector4::SetZero(void)
		{
			x = 0.0f, y = 0.0f, z = 0.0f;
		}

		bool Vector4::IsZero(void) const
		{
			return Utility::IsEqual(x, 0.0f) && Utility::IsEqual(y, 0.0f) && Utility::IsEqual(z, 0.0f) && Utility::IsEqual(w, 0.0f);
		}

		void Vector4::swap(Vector4& rhs)
		{
			std::swap(x, rhs.x);
			std::swap(y, rhs.y);
			std::swap(z, rhs.z);
			std::swap(w, rhs.w);
		}

		Vector4 operator+(Vector4 const& lhs, Vector4 const& rhs)
		{
			Vector4 res{ lhs };
			res += rhs;
			return res;
		}

		Vector4 operator-(Vector4 const& lhs, Vector4 const& rhs)
		{
			Vector4 res{ lhs };
			res -= rhs;
			return res;
		}

		Vector4 operator*(Vector4 const& lhs, f32 rhs)
		{
			Vector4 res{ lhs };
			res *= rhs;
			return res;
		}

		Vector4 operator*(f32 lhs, Vector4 const& rhs)
		{
			Vector4 res{ rhs };
			res *= lhs;
			return res;
		}

		Vector4 operator/(Vector4 const& lhs, f32 rhs)
		{
			Vector4 res{ lhs };
			res /= rhs;
			return res;
		}

		f32 Vector4Distance(Vector4 const& lhs, Vector4 const& rhs)
		{
			return sqrtf(Vector4SquareDistance(lhs, rhs));
		}

		f32 Vector4SquareDistance(Vector4 const& lhs, Vector4 const& rhs)
		{
			Vector4 res{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
			return res.LengthSq();
		}

		std::ostream& operator<<(std::ostream& os, Vector4 const& rhs)
		{
			return os << std::fixed << std::setprecision(5) << std::left << std::setw(10) << "x: " << rhs.x << " y: " << rhs.y << " z: " << rhs.z << " w: " << rhs.w;
		}
	}
}