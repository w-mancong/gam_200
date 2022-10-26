#include <pch.h>

namespace ALEngine::Math
{
	Matrix2x2::Matrix2x2(value_type value)
	{
		for (u64 i = 0; i < 2; ++i)
			(*this)(i, i) = value;
	}

	Matrix2x2::Matrix2x2(Vector2 r1, Vector2 r2)
	{
		(*this)(0, 0) = r1.x; (*this)(0, 1) = r1.y;
		(*this)(1, 0) = r2.x; (*this)(1, 1) = r2.y;
	}

	typename Matrix2x2::reference Matrix2x2::operator()(size_type row, size_type col)
	{
		return const_cast<reference>( const_cast<Matrix2x2 const&>( (*this) )(row, col) );
	}

	typename Matrix2x2::const_reference Matrix2x2::operator()(size_type row, size_type col) const
	{
#ifdef _DEBUG
		assert(0 <= row && 2 > row && 0 <= col && 2 > col && "Rows and Columns must be a positive integer lesser than 2!");
#endif
		return *(&mat[0].x + row * 2 + col);
	}

	vec2& Matrix2x2::operator()(size_type row)
	{
		return const_cast<vec2&>( const_cast<Matrix2x2 const&>( (*this) )(row) );
	}

	vec2 const& Matrix2x2::operator()(size_type row) const
	{
#ifdef _DEBUG
		assert(0 <= row && 2 > row && "Rows must be a positive integer lesser than 2!");
#endif
		return *(mat + row);
	}

	Matrix2x2& Matrix2x2::operator+=(Matrix2x2 const& rhs)
	{
		Matrix2x2 res(1.0f); Matrix2x2 const& lhs = *this;
		res(0, 0) = lhs(0, 0) + rhs(0, 0);
		res(0, 1) = lhs(0, 1) + rhs(0, 1);

		res(1, 0) = lhs(1, 0) + rhs(1, 0);
		res(1, 1) = lhs(1, 1) + rhs(1, 1);

		return (*this = res);
	}

	Matrix2x2& Matrix2x2::operator-=(Matrix2x2 const& rhs)
	{
		Matrix2x2 res(1.0f); Matrix2x2 const& lhs = *this;
		res(0, 0) = lhs(0, 0) - rhs(0, 0);
		res(0, 1) = lhs(0, 1) - rhs(0, 1);

		res(1, 0) = lhs(1, 0) - rhs(1, 0);
		res(1, 1) = lhs(1, 1) - rhs(1, 1);

		return (*this = res);
	}

	Matrix2x2& Matrix2x2::operator*=(Matrix2x2 const& rhs)
	{
		Matrix2x2 res(1.0f); Matrix2x2 const& lhs = *this;

		res(0, 0) = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0);
		res(0, 1) = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1);

		res(1, 0) = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0);
		res(1, 1) = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1);

		return (*this = res);
	}

	Matrix2x2& Matrix2x2::operator*=(value_type rhs)
	{
		Matrix2x2 res(1.0f);  Matrix2x2 const& lhs = *this;
		res(0, 0) = lhs(0, 0) * rhs;
		res(0, 1) = lhs(0, 1) * rhs;

		res(1, 0) = lhs(1, 0) * rhs;
		res(1, 1) = lhs(1, 1) * rhs;

		return (*this = res);
	}

	Matrix2x2 Matrix2x2::Inverse(void) const
	{
		return Inverse(*this);
	}

	Matrix2x2 Matrix2x2::InverseT(void) const
	{
		return InverseT(*this);
	}

	Matrix2x2 Matrix2x2::Transpose(void) const
	{
		return Transpose(*this);
	}

	f32 Matrix2x2::Determinant(void) const
	{
		return Determinant(*this);
	}

	Matrix2x2 Matrix2x2::Inverse(Matrix2x2 const& mat)
	{
#if _DEBUG
		f32 const det = Determinant(mat);
		assert(!Utility::IsEqual(det, 0.0f) && "Determinant is 0, unable to proceed due to division by 0!!");
		f32 const oneOverDeterminant = 1.0f / det;
#else
		f32 const oneOverDeterminant = 1.0f / Determinant(mat);
#endif
		vec2 const row0 = mat(0), row1 = mat(1);
		return oneOverDeterminant * mat2{ { row1.y, -row0.y }, { -row1.x, row0.x } };
	}

	Matrix2x2 Matrix2x2::InverseT(Matrix2x2 const& mat)
	{
		return Inverse(mat).Transpose();
	}

	Matrix2x2 Matrix2x2::Transpose(Matrix2x2 const& mat)
	{
		return mat2
		{
			vec2{ mat(0, 0), mat(1, 0) },
			vec2{ mat(0, 1), mat(1, 1) }
		};
	}

	f32 Matrix2x2::Determinant(Matrix2x2 const& mat)
	{
		vec2 const row0 = mat(0), row1 = mat(1);
		return row0.x * row1.y - row1.x * row0.y;
	}

	Matrix2x2 operator+(Matrix2x2 const& lhs, Matrix2x2 const& rhs)
	{
		Matrix2x2 res{ lhs }; res += rhs;
		return res;
	}

	Matrix2x2 operator-(Matrix2x2 const& lhs, Matrix2x2 const& rhs)
	{
		Matrix2x2 res{ lhs }; res -= rhs;
		return res;
	}

	Matrix2x2 operator*(Matrix2x2 const& lhs, Matrix2x2 const& rhs)
	{
		Matrix2x2 res{ lhs }; res *= rhs;
		return res;
	}

	Matrix2x2 operator*(Matrix2x2 const& lhs, typename Matrix2x2::value_type rhs)
	{
		Matrix2x2 res{ lhs }; res *= rhs;
		return res;
	}

	Matrix2x2 operator*(typename Matrix2x2::value_type lhs, Matrix2x2 const& rhs)
	{
		Matrix2x2 res{ rhs }; res *= lhs;
		return res;
	}

	Vector2 operator*(Matrix2x2 const& lhs, Vector2 const& rhs)
	{
		f32 x{}, y{};
		x = lhs(0, 0) * rhs.x + lhs(0, 1) * rhs.y;
		y = lhs(1, 0) * rhs.x + lhs(1, 1) * rhs.y;

		return Vector2{ x, y };
	}

	std::ostream& operator<<(std::ostream& os, Matrix2x2 const& rhs)
	{
		os << std::fixed << std::setprecision(5);
		u64 constexpr MAX{ 2 };
		for (u64 i = 0; i < MAX; ++i)
		{
			for (u64 j = 0; j < MAX; ++j)
				os << std::left << std::setw(10) << rhs(i, j) << (j + 1 == MAX ? '\0' : ' ');
			os << std::endl;
		}
		return os;
	}
}