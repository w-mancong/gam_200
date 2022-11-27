/*!
file:	Vector2Int.cpp
author: Chan Jie Ming Stanley
email:	c.jiemingstanley\@digipen.edu
brief:	This file contain class definition for representing a 2D vector and points using
integers where the precision of floating-point is not required
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Math
{

	Vector2Int const Vector2Int::right = Vector2Int(1, 0);
	Vector2Int const Vector2Int::left = Vector2Int(-1, 0);
	Vector2Int const Vector2Int::up = Vector2Int(0, 1);
	Vector2Int const Vector2Int::down = Vector2Int(0, -1);
	Vector2Int const Vector2Int::one = Vector2Int(1, 1);
	Vector2Int const Vector2Int::zero = Vector2Int(0, 0);

	Vector2Int::Vector2Int(void) : x{ 0 }, y{ 0 } {}

	Vector2Int::Vector2Int(s32 x, s32 y) : x{ x }, y{ y } {}

	Vector2Int::Vector2Int(Vector3Int const& rhs) : x{ rhs.x }, y{ rhs.y } {}

	Vector2Int& Vector2Int::operator=(Vector3Int const& rhs)
	{
		Vector2Int tmp{ rhs };
		swap(tmp);
		return *this;
	}

	/*********************************************************************************
										MATH ARITHMETIC
	*********************************************************************************/
	Vector2Int& Vector2Int::operator+=(Vector2Int const& rhs)
	{
		x += rhs.x, y += rhs.y;
		return *this;
	}

	Vector2Int& Vector2Int::operator-=(Vector2Int const& rhs)
	{
		x -= rhs.x, y -= rhs.y;
		return *this;
	}

	Vector2Int& Vector2Int::operator*=(s32 rhs)
	{
		x *= rhs, y *= rhs;
		return *this;
	}

	Vector2Int& Vector2Int::operator/=(s32 rhs)
	{
		x /= rhs, y /= rhs;
		return *this;
	}

	Vector2Int Vector2Int::operator-(void) const
	{
		return Vector2Int(-x, -y);
	}

	s32 Vector2Int::Dot(Vector2Int const& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	s32 Vector2Int::Magnitude(void) const
	{
		return static_cast<s32>(sqrt(MagnitudeSq()));
	}

	s32 Vector2Int::MagnitudeSq(void) const
	{
		return Dot(*this);
	}

	/*********************************************************************************
										UTILITIES
	*********************************************************************************/
	bool Vector2Int::operator==(Vector2Int const& rhs) const
	{
		const s32 abs_ax = abs(x);
		const s32 abs_bx = abs(rhs.x);

		const s32 abs_ay = abs(y);
		const s32 abs_by = abs(rhs.y);

		if ((abs_ax == abs_bx) && (abs_ay == abs_by))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Vector2Int::operator!=(Vector2Int const& rhs) const
	{
		return !(*this == rhs);
	}

	void Vector2Int::SetZero(void)
	{
		x = 0, y = 0;
	}

	bool Vector2Int::IsZero(void) const
	{
		if ((x == 0) && (y == 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Vector2Int::swap(Vector2Int& rhs)
	{
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);
	}

	/*********************************************************************************
									STATIC FUNCTIONS
	*********************************************************************************/
	s32 Vector2Int::Distance(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		Vector2Int tmp{ lhs - rhs };
		return tmp.Magnitude();
	}

	s32 Vector2Int::DistanceSq(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		Vector2Int tmp{ lhs - rhs };
		return tmp.MagnitudeSq();
	}

	s32 Vector2Int::Dot(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		return lhs.Dot(rhs);
	}

	Vector2Int Vector2Int::Max(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		return Vector2Int(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
	}

	Vector2Int Vector2Int::Min(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		return Vector2Int(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
	}

	Vector2Int operator+(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		Vector2Int res{ lhs }; res += rhs;
		return res;
	}

	Vector2Int operator-(Vector2Int const& lhs, Vector2Int const& rhs)
	{
		Vector2Int res{ lhs }; res -= rhs;
		return res;
	}

	Vector2Int operator*(Vector2Int const& lhs, s32 rhs)
	{
		Vector2Int res{ lhs }; res *= rhs;
		return res;
	}

	Vector2Int operator*(s32 lhs, Vector2Int const& rhs)
	{
		Vector2Int res{ rhs }; res *= lhs;
		return res;
	}

	Vector2Int operator/(Vector2Int const& lhs, s32 rhs)
	{
		Vector2Int res{ lhs }; res /= rhs;
		return res;
	}

	std::ostream& operator<<(std::ostream& os, Vector2Int const& rhs)
	{
		return os << std::left << std::setw(10) << "x: " << rhs.x << " y: " << rhs.y;
	}
}