#include "pch.h"

namespace ALEngine::Math
{
	Vector3Int const Vector3Int::right = Vector3Int(1, 0, 0);
	Vector3Int const Vector3Int::left = Vector3Int(-1, 0, 0);
	Vector3Int const Vector3Int::up = Vector3Int(0, 1, 0);
	Vector3Int const Vector3Int::down = Vector3Int(0, -1, 0);
	Vector3Int const Vector3Int::forward = Vector3Int(0, 0, 1);
	Vector3Int const Vector3Int::back = Vector3Int(0, 0, -1);
	Vector3Int const Vector3Int::one = Vector3Int(1, 1, 1);
	Vector3Int const Vector3Int::zero = Vector3Int(0, 0, 0);

	Vector3Int::Vector3Int(void): x { 0 }, y{ 0 }, z{ 0 } {}
	Vector3Int::Vector3Int(s32 x, s32 y, s32 z) : x{ x }, y{ y }, z{ z } {}
	Vector3Int::Vector3Int(Vector2Int const& rhs) : x{ rhs.x }, y{ rhs.y }, z{ 0 } {}

	Vector3Int& Vector3Int::operator=(Vector2Int const& rhs)
	{
		Vector3Int tmp{ rhs };
		std::swap(*this, tmp);
		return *this;
	}

	/*********************************************************************************
								MATH ARITHMETIC
	*********************************************************************************/
	Vector3Int& Vector3Int::operator+=(Vector3Int const& rhs)
	{
		x += rhs.x, y += rhs.y, z += rhs.z;
		return *this;
	}

	Vector3Int& Vector3Int::operator-=(Vector3Int const& rhs)
	{
		x -= rhs.x, y -= rhs.y, z -= rhs.z;
		return *this;
	}

	Vector3Int& Vector3Int::operator*=(s32 rhs)
	{
		x *= rhs, y *= rhs, z *= rhs;
		return *this;
	}

	Vector3Int& Vector3Int::operator/=(s32 rhs)
	{
		x /= rhs, y /= rhs, z / rhs;
		return *this;
	}

	Vector3Int Vector3Int::operator-(void) const
	{
		return Vector3Int(-x, -y, -z);
	}

	s32 Vector3Int::Dot(Vector3Int const& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	s32 Vector3Int::Magnitude(void) const
	{
		return static_cast<s32>(sqrt(MagnitudeSq()));
	}

	s32 Vector3Int::MagnitudeSq(void) const
	{
		return Dot(*this);
	}

	Vector3Int Vector3Int::Cross(Vector3Int const& rhs) const
	{
		Vector3Int res;
		res.x = y * rhs.z - z * rhs.y;
		res.y = z * rhs.x - x * rhs.z;
		res.z = x * rhs.y - y * rhs.x;
		return res;
	}

	/*********************************************************************************
											UTILITIES
	*********************************************************************************/
	bool Vector3Int::operator==(Vector3Int const& rhs) const
	{
		const s32 abs_ax = abs(x);
		const s32 abs_bx = abs(rhs.x);

		const s32 abs_ay = abs(y);
		const s32 abs_by = abs(rhs.y);

		const s32 abs_az = abs(z);
		const s32 abs_bz = abs(rhs.z);

		if ((abs_ax == abs_bx) && (abs_ay == abs_by) && (abs_az == abs_bz))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Vector3Int::operator!=(Vector3Int const& rhs) const
	{
		return !(*this == rhs);
	}

	void Vector3Int::SetZero(void)
	{
		x = 0, y = 0, z = 0;
	}

	bool Vector3Int::IsZero(void) const
	{
		if ((x == 0) && (y == 0) && (z == 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	s32 Vector3Int::Distance(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		Vector3Int tmp{ lhs - rhs };
		return tmp.Magnitude();
	}

	s32 Vector3Int::DistanceSq(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		Vector3Int tmp{ lhs - rhs };
		return tmp.MagnitudeSq();
	}

	s32 Vector3Int::Dot(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		return lhs.Dot(rhs);
	}

	Vector3Int Vector3Int::Max(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		return Vector3Int(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
	}

	Vector3Int Vector3Int::Min(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		return Vector3Int(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
	}

	Vector3Int Vector3Int::Cross(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		return lhs.Cross(rhs);
	}

	void Vector3Int::swap(Vector3Int& rhs)
	{
		std::swap(x, rhs.x);
		std::swap(y, rhs.y);
		std::swap(z, rhs.z);
	}

	Vector3Int operator+(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		Vector3Int res{ lhs };
		res += rhs;
		return res;
	}

	Vector3Int operator-(Vector3Int const& lhs, Vector3Int const& rhs)
	{
		Vector3Int res{ lhs };
		res -= rhs;
		return res;
	}

	Vector3Int operator*(Vector3Int const& lhs, s32 rhs)
	{
		Vector3Int res{ lhs };
		res *= rhs;
		return res;
	}

	Vector3Int operator*(s32 lhs, Vector3Int const& rhs)
	{
		Vector3Int res{ rhs };
		res *= lhs;
		return res;
	}

	Vector3Int operator/(Vector3Int const& lhs, s32 rhs)
	{
		Vector3Int res{ lhs };
		res /= rhs;
		return res;
	}

	std::ostream& operator<<(std::ostream& os, Vector3Int const& rhs)
	{
		return os << std::left << "x: " << std::setw(10) << rhs.x << "y: " << std::setw(10) << rhs.y << "z: " << std::setw(10) << rhs.z;
	}
}