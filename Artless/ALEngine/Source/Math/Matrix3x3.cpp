/*!
file:	Matrix3x3.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain a 3x3 Matrix that is inherited from a base matrix class. The file
		also provides function declaration to constructing a Translation, Scale and Rotation matrix

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
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
#ifndef NDEBUG
		assert(0 <= row && 3 > row && 0 <= col && 3 > col && "Rows and Columns must be a positive integer lesser than 3!");
#endif
		return *(&mat[0].x + row * 3 + col);
	}

	vec3& Matrix3x3::operator()(size_type row)
	{
		return const_cast<vec3&>( const_cast<Matrix3x3 const&>( (*this) )(row) );
	}

	vec3 const& Matrix3x3::operator()(size_type row) const
	{
#ifndef NDEBUG
		assert(0 <= row && 3 > row && "Rows must be a positive integer lesser than 3!");
#endif
		return *(mat + row);
	}

	Matrix3x3 Matrix3x3::Inverse(void) const
	{
		return Inverse(*this);
	}

	Matrix3x3 Matrix3x3::InverseT(void) const
	{
		return InverseT(*this);
	}

	Matrix3x3 Matrix3x3::Transpose(void) const
	{
		return Transpose(*this);
	}

	f32 Matrix3x3::Determinant(void) const
	{
		return Determinant(*this);
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

	Matrix3x3 Matrix3x3::Inverse(Matrix3x3 const& mat)
	{
#ifndef NDEBUG
		f32 const det = Determinant(mat);
		assert(!Utility::IsEqual(det, 0.0f) && "Determinant is 0, unable to proceed due to division by 0!!");
		f32 const oneOverDeterminant = 1.0f / det;
#else
		f32 const oneOverDeterminant = 1.0f / Determinant(mat);
#endif
		mat3 adj{ 1.0f }; mat3 const transposed{ mat.Transpose() };
		vec4 const row0 = transposed(0), row1 = transposed(1), row2 = transposed(2);

		mat2 const mat00{  vec2{ row1.y, row1.z },  vec2{ row2.y, row2.z } },
				   mat01{ -vec2{ row1.x, row1.z }, -vec2{ row2.x, row2.z } },
				   mat02{  vec2{ row1.x, row1.y },  vec2{ row2.x, row2.y } };

		mat2 const mat10{ -vec2{ row0.y, row0.z }, -vec2{ row2.y, row2.z }},
				   mat11{  vec2{ row0.x, row0.z },  vec2{ row2.x, row2.z }},
				   mat12{ -vec2{ row0.x, row0.y }, -vec2{ row2.x, row2.y }};

		mat2 const mat20{  vec2{ row0.y, row0.z },  vec2{ row1.y, row1.z } },
				   mat21{ -vec2{ row0.x, row0.z }, -vec2{ row1.x, row1.z } },
				   mat22{  vec2{ row0.x, row0.y },  vec2{ row1.x, row1.y } };

		adj(0, 0) = mat00.Determinant(), adj(0, 1) = mat01.Determinant(), adj(0, 2) = mat02.Determinant();
		adj(1, 0) = mat10.Determinant(), adj(1, 1) = mat11.Determinant(), adj(1, 2) = mat12.Determinant();
		adj(2, 0) = mat20.Determinant(), adj(2, 1) = mat21.Determinant(), adj(2, 2) = mat22.Determinant();

		return oneOverDeterminant * adj;
	}

	Matrix3x3 Matrix3x3::InverseT(Matrix3x3 const& mat)
	{
		return Inverse(mat).Transpose();
	}

	Matrix3x3 Matrix3x3::Transpose(Matrix3x3 const& mat)
	{
		return mat3
		{
			vec3{ mat(0, 0), mat(1, 0), mat(2, 0) },
			vec3{ mat(0, 1), mat(1, 1), mat(2, 1) },
			vec3{ mat(0, 2), mat(1, 2), mat(2, 2) },
		};
	}

	f32 Matrix3x3::Determinant(Matrix3x3 const& mat)
	{
		vec3 const row0 = mat(0), row1 = mat(1), row2 = mat(2);
		return row0.x * (row1.y * row2.z - row2.y * row1.z) - row0.y * (row1.x * row2.z - row2.x * row1.z) + row0.z * (row1.x * row2.y - row2.x * row1.y);
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