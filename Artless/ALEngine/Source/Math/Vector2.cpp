/*!
file:	Vector2.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain class definition for representing a 2D vector

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Math
{
	Vector2 const Vector2::right			= Vector2(1.0f, 0.0f);
	Vector2 const Vector2::left				= Vector2(-1.0f, 0.0f);
	Vector2 const Vector2::up				= Vector2(0.0f, 1.0f);
	Vector2 const Vector2::down				= Vector2(0.0f, -1.0f);
	Vector2 const Vector2::one				= Vector2(1.0f, 1.0f);
	Vector2 const Vector2::zero				= Vector2(0.0f, 0.0f);
	Vector2 const Vector2::positiveInfinity = Vector2(std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
	Vector2 const Vector2::negativeInfinity = Vector2(-std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());

	Vector2::Vector2(void) : x{ 0.0f }, y{ 0.0f } {}
	Vector2::Vector2(f32 x, f32 y) : x{ x }, y{ y } {}
	Vector2::Vector2(Vector3 const& rhs) : x{ rhs.x }, y{ rhs.y } {}
	Vector2::Vector2(Vector4 const& rhs) : x{ rhs.x }, y{ rhs.y } {}
		
	Vector2& Vector2::operator=(Vector3 const& rhs)
	{
		Vector2 tmp{ rhs };
		swap(tmp);
		return *this;
	}

	Vector2& Vector2::operator=(Vector4 const& rhs)
	{
		Vector2 tmp{ rhs };
		swap(tmp);
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

	Vector2& Vector2::operator*=(f32 rhs)
	{
		x *= rhs, y *= rhs;
		return *this;
	}

	Vector2& Vector2::operator/=(f32 rhs)
	{
		x /= rhs, y /= rhs;
		return *this;
	}

	Vector2 Vector2::operator-(void) const
	{
		return Vector2(-x, -y);
	}

	f32 Vector2::Dot(Vector2 const& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	f32 Vector2::Magnitude(void) const
	{
		return sqrtf(MagnitudeSq());
	}

	f32 Vector2::MagnitudeSq(void) const
	{
		return Dot(*this);
	}

	void Vector2::Normalized(void)
	{
		f32 const inv_len = Q_rsqrt(MagnitudeSq());
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

	void Vector2::swap(Vector2& rhs)
	{
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);
	}

	/*********************************************************************************
									STATIC FUNCTIONS
	*********************************************************************************/
	f32 Vector2::Angle(Vector2 const& from, Vector2 const& to)
	{
		return RadianToDegree( std::acosf( Vector2::Dot(from, to) / ( from.Magnitude() * to.Magnitude() ) ) );
	}

	Vector2 Vector2::ClampMagnitude(Vector2 const& lhs, f32 maxLength)
	{
		Vector2 tmp{ lhs };
		return tmp.Normalize() * maxLength;
	}

	f32 Vector2::Distance(Vector2 const& lhs, Vector2 const& rhs)
	{
		Vector2 tmp{ lhs - rhs };
		return tmp.Magnitude();
	}

	f32 Vector2::DistanceSq(Vector2 const& lhs, Vector2 const& rhs)
	{
		Vector2 tmp{ lhs - rhs };
		return tmp.MagnitudeSq();
	}

	f32 Vector2::Dot(Vector2 const& lhs, Vector2 const& rhs)
	{
		return lhs.Dot(rhs);
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

	Vector2 operator*(Vector2 const& lhs, f32 rhs)
	{
		Vector2 res{ lhs }; res *= rhs;
		return res;
	}

	Vector2 operator*(f32 lhs, Vector2 const& rhs)
	{
		Vector2 res{ rhs }; res *= lhs;
		return res;
	}

	Vector2 operator/(Vector2 const& lhs, f32 rhs)
	{
		Vector2 res{ lhs }; res /= rhs;
		return res;
	}

	std::ostream& operator<<(std::ostream& os, Vector2 const& rhs)
	{
		return os << std::fixed << std::setprecision(5) << std::left << std::setw(10) << "x: " << rhs.x << " y: " << rhs.y;
	}
}