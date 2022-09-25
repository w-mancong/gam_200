#include "pch.h"

namespace ALEngine::Math
{
	Matrix3x3::Matrix3x3(value_type value)
	{
		for (u64 i = 0; i < 3; ++i)
			(*this)(i, i) = value;
	}

	Matrix3x3::Matrix3x3(Vector3 r1, Vector3 r2, Vector3 r3)
	{
		(*this)(0, 0) = r1.x; (*this)(0, 1) = r1.y; (*this)(0, 2) = r1.z;
		(*this)(1, 0) = r2.x; (*this)(1, 1) = r2.y; (*this)(1, 2) = r2.z;
		(*this)(2, 0) = r3.x; (*this)(2, 1) = r3.y; (*this)(2, 2) = r3.z;
	}

	Matrix3x3& Matrix3x3::operator+=(Matrix3x3 const& rhs)
	{
		Matrix3x3 res(1.0f); Matrix3x3 const& lhs = *this;
		res(0, 0) = lhs(0, 0) + rhs(0, 0);
		res(0, 1) = lhs(0, 1) + rhs(0, 1);
		res(0, 2) = lhs(0, 2) + rhs(0, 2);

		res(1, 0) = lhs(1, 0) + rhs(1, 0);
		res(1, 1) = lhs(1, 1) + rhs(1, 1);
		res(1, 2) = lhs(1, 2) + rhs(1, 2);

		res(2, 0) = lhs(2, 0) + rhs(2, 0);
		res(2, 1) = lhs(2, 1) + rhs(2, 1);
		res(2, 2) = lhs(2, 2) + rhs(2, 2);

		return (*this = res);
	}

	Matrix3x3& Matrix3x3::operator-=(Matrix3x3 const& rhs)
	{
		Matrix3x3 res(1.0f); Matrix3x3 const& lhs = *this;
		res(0, 0) = lhs(0, 0) - rhs(0, 0);
		res(0, 1) = lhs(0, 1) - rhs(0, 1);
		res(0, 2) = lhs(0, 2) - rhs(0, 2);

		res(1, 0) = lhs(1, 0) - rhs(1, 0);
		res(1, 1) = lhs(1, 1) - rhs(1, 1);
		res(1, 2) = lhs(1, 2) - rhs(1, 2);

		res(2, 0) = lhs(2, 0) - rhs(2, 0);
		res(2, 1) = lhs(2, 1) - rhs(2, 1);
		res(2, 2) = lhs(2, 2) - rhs(2, 2);

		return (*this = res);
	}

	Matrix3x3& Matrix3x3::operator*=(Matrix3x3 const& rhs)
	{
		Matrix3x3 res(1.0f); Matrix3x3 const& lhs = *this;

		res(0, 0) = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0);
		res(0, 1) = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1);
		res(0, 2) = lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2);

		res(1, 0) = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0);
		res(1, 1) = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1);
		res(1, 2) = lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2);

		res(2, 0) = lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0);
		res(2, 1) = lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1);
		res(2, 2) = lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2);

		return (*this = res);
	}

	Matrix3x3& Matrix3x3::operator*=(value_type rhs)
	{
		Matrix3x3 res(1.0f);  Matrix3x3 const& lhs = *this;
		res(0, 0) = lhs(0, 0) * rhs;
		res(0, 1) = lhs(0, 1) * rhs;
		res(0, 2) = lhs(0, 2) * rhs;

		res(1, 0) = lhs(1, 0) * rhs;
		res(1, 1) = lhs(1, 1) * rhs;
		res(1, 2) = lhs(1, 2) * rhs;

		res(2, 0) = lhs(2, 0) * rhs;
		res(2, 1) = lhs(2, 1) * rhs;
		res(2, 2) = lhs(2, 2) * rhs;

		return (*this = res);
	}

	Matrix3x3::reference Matrix3x3::operator()(size_type row, size_type col)
	{
		return const_cast<reference>(const_cast<Matrix3x3 const&>((*this))(row, col));
	}

	Matrix3x3::const_reference Matrix3x3::operator()(size_type row, size_type col) const
	{
#ifdef _DEBUG
		assert(0 <= row && 3 > row && 0 <= col && 3 > col, "Rows and Columns must be a positive integer lesser than 3!");
#endif
		return *(&mat[0].x + row * 3 + col);
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
		f32 x{}, y{};
		x = lhs(0, 0) * rhs.x + lhs(0, 1) * rhs.y + lhs(0, 2);
		y = lhs(1, 0) * rhs.x + lhs(1, 1) * rhs.y + lhs(1, 2);

		return Vector2{ x, y };
	}

	Vector3 operator*(Matrix3x3 const& lhs, Vector3 const& rhs)
	{
		f32 x{}, y{}, z{};
		x = lhs(0, 0) * rhs.x + lhs(0, 1) * rhs.y + lhs(0, 2) * rhs.z;
		y = lhs(1, 0) * rhs.x + lhs(1, 1) * rhs.y + lhs(1, 2) * rhs.z;
		z = lhs(2, 0) * rhs.x + lhs(2, 1) * rhs.y + lhs(2, 2) * rhs.z;

		return Vector3{ x, y, z };
	}

	std::ostream& operator<<(std::ostream& os, Matrix3x3 const& rhs)
	{
		os << std::fixed << std::setprecision(5);
		u64 constexpr MAX{ 3 };
		for (u64 i = 0; i < MAX; ++i)
		{
			for (u64 j = 0; j < MAX; ++j)
				os << std::left << std::setw(10) << rhs(i, j) << (j + 1 == MAX ? '\0' : ' ');
			os << std::endl;
		}
		return os;
	}
}