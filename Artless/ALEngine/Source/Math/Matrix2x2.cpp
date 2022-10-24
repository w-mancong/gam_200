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

	}

	Matrix2x2& Matrix2x2::operator*=(value_type rhs)
	{

	}

	Matrix2x2 Matrix2x2::Inverse(void) const
	{

	}

	Matrix2x2 Matrix2x2::InverseT(void) const
	{

	}

	Matrix2x2 Matrix2x2::Transpose(void) const
	{

	}

	f32 Matrix2x2::Determinant(void) const
	{

	}

	Matrix2x2 Matrix2x2::Inverse(Matrix2x2 const& mat)
	{

	}

	Matrix2x2 Matrix2x2::InverseT(Matrix2x2 const& mat)
	{

	}

	Matrix2x2 Matrix2x2::Transpose(Matrix2x2 const& mat)
	{

	}

	f32 Matrix2x2::Determinant(Matrix2x2 const& mat)
	{

	}

	Matrix2x2 operator+(Matrix2x2 const& lhs, Matrix2x2 const& rhs)
	{

	}

	Matrix2x2 operator-(Matrix2x2 const& lhs, Matrix2x2 const& rhs)
	{

	}

	Matrix2x2 operator*(Matrix2x2 const& lhs, Matrix2x2 const& rhs)
	{

	}

	Matrix2x2 operator*(Matrix2x2 const& lhs, typename Matrix2x2::value_type rhs)
	{

	}

	Matrix2x2 operator*(typename Matrix2x2::value_type lhs, Matrix2x2 const& rhs)
	{

	}

	Vector2 operator*(Matrix2x2 const& lhs, Vector2 const& rhs)
	{

	}

	std::ostream& operator<<(std::ostream& os, Matrix4x4 const& rhs)
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