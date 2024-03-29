/*!
file:	Matrix4x4.cpp
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain a 4x4 Matrix that is inherited from a base matrix class. The file
		also provides function declaration to constructing a Translation, Scale and Rotation matrix
		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Math
{
	Matrix4x4::Matrix4x4(value_type value)
	{
		for (u64 i = 0; i < 4; ++i)
			(*this)(i, i) = value;
	}

	Matrix4x4::Matrix4x4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4)
	{
		(*this)(0, 0) = r1.x; (*this)(0, 1) = r1.y; (*this)(0, 2) = r1.z; (*this)(0, 3) = r1.w;
		(*this)(1, 0) = r2.x; (*this)(1, 1) = r2.y; (*this)(1, 2) = r2.z; (*this)(1, 3) = r2.w;
		(*this)(2, 0) = r3.x; (*this)(2, 1) = r3.y; (*this)(2, 2) = r3.z; (*this)(2, 3) = r3.w;
		(*this)(3, 0) = r4.x; (*this)(3, 1) = r4.y; (*this)(3, 2) = r4.z; (*this)(3, 3) = r4.w;
	}

	Matrix4x4::reference Matrix4x4::operator()(size_type row, size_type col)
	{
		return const_cast<reference>(const_cast<Matrix4x4 const&>((*this))(row, col));
	}

	Matrix4x4::const_reference Matrix4x4::operator()(size_type row, size_type col) const
	{
#ifndef NDEBUG
		assert(0 <= row && 4 > row && 0 <= col && 4 > col && "Rows and Columns must be a positive integer lesser than 4!");
#endif
		return *(value_ptr() + row * 4 + col);
	}

	vec4& Matrix4x4::operator()(size_type row)
	{
		return const_cast<vec4&>(const_cast<Matrix4x4 const&>((*this))(row));
	}

	vec4 const& Matrix4x4::operator()(size_type row) const
	{
#if EDITOR
		assert(0 <= row && 4 > row && "Rows must be a positive integer lesser than 4!");
#endif
		return *(mat + row);
	}

	vec4 Matrix4x4::Column(size_type col) const
	{
#if	EDITOR
		assert(0 <= col && 4 > col && "Columns must be a positive integer lesser than 4!");
#endif
		return vec4
		{
			(*this)(0, col),
			(*this)(1, col),
			(*this)(2, col),
			(*this)(3, col),
		};
	}

	Matrix4x4& Matrix4x4::operator+=(Matrix4x4 const& rhs)
	{
		Matrix4x4 res(1.0f);  Matrix4x4 const& lhs = *this;
		res(0, 0) = lhs(0, 0) + rhs(0, 0);
		res(0, 1) = lhs(0, 1) + rhs(0, 1);
		res(0, 2) = lhs(0, 2) + rhs(0, 2);
		res(0, 3) = lhs(0, 3) + rhs(0, 3);

		res(1, 0) = lhs(1, 0) + rhs(1, 0);
		res(1, 1) = lhs(1, 1) + rhs(1, 1);
		res(1, 2) = lhs(1, 2) + rhs(1, 2);
		res(1, 3) = lhs(1, 3) + rhs(1, 3);

		res(2, 0) = lhs(2, 0) + rhs(2, 0);
		res(2, 1) = lhs(2, 1) + rhs(2, 1);
		res(2, 2) = lhs(2, 2) + rhs(2, 2);
		res(2, 3) = lhs(2, 3) + rhs(2, 3);

		res(3, 0) = lhs(3, 0) + rhs(3, 0);
		res(3, 1) = lhs(3, 1) + rhs(3, 1);
		res(3, 2) = lhs(3, 2) + rhs(3, 2);
		res(3, 3) = lhs(3, 3) + rhs(3, 3);
		return (*this = res);
	}

	Matrix4x4& Matrix4x4::operator-=(Matrix4x4 const& rhs)
	{
		Matrix4x4 res(1.0f);  Matrix4x4 const& lhs = *this;
		res(0, 0) = lhs(0, 0) - rhs(0, 0);
		res(0, 1) = lhs(0, 1) - rhs(0, 1);
		res(0, 2) = lhs(0, 2) - rhs(0, 2);
		res(0, 3) = lhs(0, 3) - rhs(0, 3);

		res(1, 0) = lhs(1, 0) - rhs(1, 0);
		res(1, 1) = lhs(1, 1) - rhs(1, 1);
		res(1, 2) = lhs(1, 2) - rhs(1, 2);
		res(1, 3) = lhs(1, 3) - rhs(1, 3);

		res(2, 0) = lhs(2, 0) - rhs(2, 0);
		res(2, 1) = lhs(2, 1) - rhs(2, 1);
		res(2, 2) = lhs(2, 2) - rhs(2, 2);
		res(2, 3) = lhs(2, 3) - rhs(2, 3);

		res(3, 0) = lhs(3, 0) - rhs(3, 0);
		res(3, 1) = lhs(3, 1) - rhs(3, 1);
		res(3, 2) = lhs(3, 2) - rhs(3, 2);
		res(3, 3) = lhs(3, 3) - rhs(3, 3);
		return (*this = res);
	}

	Matrix4x4& Matrix4x4::operator*=(Matrix4x4 const& rhs)
	{
		Matrix4x4 res(1.0f); Matrix4x4 const& lhs = *this;

		res(0, 0) = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0) + lhs(0, 3) * rhs(3, 0);
		res(0, 1) = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1) + lhs(0, 3) * rhs(3, 1);
		res(0, 2) = lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2) + lhs(0, 3) * rhs(3, 2);
		res(0, 3) = lhs(0, 0) * rhs(0, 3) + lhs(0, 1) * rhs(1, 3) + lhs(0, 2) * rhs(2, 3) + lhs(0, 3) * rhs(3, 3);

		res(1, 0) = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0) + lhs(1, 3) * rhs(3, 0);
		res(1, 1) = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1) + lhs(1, 3) * rhs(3, 1);
		res(1, 2) = lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2) + lhs(1, 3) * rhs(3, 2);
		res(1, 3) = lhs(1, 0) * rhs(0, 3) + lhs(1, 1) * rhs(1, 3) + lhs(1, 2) * rhs(2, 3) + lhs(1, 3) * rhs(3, 3);

		res(2, 0) = lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0) + lhs(2, 3) * rhs(3, 0);
		res(2, 1) = lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1) + lhs(2, 3) * rhs(3, 1);
		res(2, 2) = lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2) + lhs(2, 3) * rhs(3, 2);
		res(2, 3) = lhs(2, 0) * rhs(0, 3) + lhs(2, 1) * rhs(1, 3) + lhs(2, 2) * rhs(2, 3) + lhs(2, 3) * rhs(3, 3);

		res(3, 0) = lhs(3, 0) * rhs(0, 0) + lhs(3, 1) * rhs(1, 0) + lhs(3, 2) * rhs(2, 0) + lhs(3, 3) * rhs(3, 0);
		res(3, 1) = lhs(3, 0) * rhs(0, 1) + lhs(3, 1) * rhs(1, 1) + lhs(3, 2) * rhs(2, 1) + lhs(3, 3) * rhs(3, 1);
		res(3, 2) = lhs(3, 0) * rhs(0, 2) + lhs(3, 1) * rhs(1, 2) + lhs(3, 2) * rhs(2, 2) + lhs(3, 3) * rhs(3, 2);
		res(3, 3) = lhs(3, 0) * rhs(0, 3) + lhs(3, 1) * rhs(1, 3) + lhs(3, 2) * rhs(2, 3) + lhs(3, 3) * rhs(3, 3);

		return (*this = res);
	}

	Matrix4x4& Matrix4x4::operator*=(value_type rhs)
	{
		Matrix4x4 res(1.0f);  Matrix4x4 const& lhs = *this;
		res(0, 0) = lhs(0, 0) * rhs;
		res(0, 1) = lhs(0, 1) * rhs;
		res(0, 2) = lhs(0, 2) * rhs;
		res(0, 3) = lhs(0, 3) * rhs;

		res(1, 0) = lhs(1, 0) * rhs;
		res(1, 1) = lhs(1, 1) * rhs;
		res(1, 2) = lhs(1, 2) * rhs;
		res(1, 3) = lhs(1, 3) * rhs;

		res(2, 0) = lhs(2, 0) * rhs;
		res(2, 1) = lhs(2, 1) * rhs;
		res(2, 2) = lhs(2, 2) * rhs;
		res(2, 3) = lhs(2, 3) * rhs;

		res(3, 0) = lhs(3, 0) * rhs;
		res(3, 1) = lhs(3, 1) * rhs;
		res(3, 2) = lhs(3, 2) * rhs;
		res(3, 3) = lhs(3, 3) * rhs;

		return (*this = res);
	}

	typename Matrix4x4::const_pointer Matrix4x4::value_ptr(void) const
	{
		return &mat[0].x;
	}

	Matrix4x4 Matrix4x4::Inverse(void) const
	{
		return Inverse(*this);
	}

	Matrix4x4 Matrix4x4::InverseT(void) const
	{
		return InverseT(*this);
	}

	Matrix4x4 Matrix4x4::Transpose(void) const
	{
		return Transpose(*this);
	}

	f32 Matrix4x4::Determinant(void) const
	{
		return Determinant(*this);
	}

	Matrix4x4 Matrix4x4::Translate(f32 x, f32 y, f32 z)
	{
		Matrix4x4 res(1.0f); res(3, 0) = x, res(3, 1) = y, res(3, 2) = z;
		return res;
	}

	Matrix4x4 Matrix4x4::Translate(Vector3 const& rhs)
	{
		return Translate(rhs.x, rhs.y, rhs.z);
	}

	Matrix4x4 Matrix4x4::Scale(f32 x, f32 y, f32 z)
	{
		Matrix4x4 res(1.0f); res(0, 0) = x, res(1, 1) = y, res(2, 2) = z;
		return res;
	}

	Matrix4x4 Matrix4x4::Scale(Vector3 const& rhs)
	{
		return Scale(rhs.x, rhs.y, rhs.z);
	}

	Matrix4x4 Matrix4x4::Rotation(f32 angle, Vector3 const& axis)
	{
		Matrix4x4 res;
		f32 const rad = DegreeToRadian(angle);
		f32 const cos = std::cosf(rad), sin = std::sinf(rad), omc = 1.0f - cos;

		res(0, 0) = axis.x * axis.x * omc + cos;
		res(0, 1) = axis.x * axis.y * omc + axis.z * sin;
		res(0, 2) = axis.x * axis.z * omc - axis.y * sin;

		res(1, 0) = axis.x * axis.y * omc - axis.z * sin;
		res(1, 1) = axis.y * axis.y * omc + cos;
		res(1, 2) = axis.y * axis.z * omc + axis.x * sin;

		res(2, 0) = axis.x * axis.z * omc + axis.y * sin;
		res(2, 1) = axis.y * axis.z * omc - axis.x * sin;
		res(2, 2) = axis.z * axis.z * omc + cos;

		return res;
	}

	Matrix4x4 Matrix4x4::Ortho(f32 left, f32 right, f32 bottom, f32 top)
	{
		Matrix4x4 res{ 1.0f };
		res(0, 0) = 2.0f / (right - left);
		res(1, 1) = 2.0f / (top - bottom);
		res(2, 2) = -1.0f;
		res(3, 0) = -(right + left) / (right - left);
		res(3, 1) = -(top + bottom) / (top - bottom);
		return res;
	}

	Matrix4x4 Matrix4x4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
	{
		Matrix4x4 res{ 1.0f };
		res(0, 0) = 2.0f / (right - left);
		res(1, 1) = 2.0f / (top - bottom);
		res(2, 2) = -2.0f / (zFar - zNear);
		res(3, 0) = -(right + left) / (right - left);
		res(3, 1) = -(top + bottom) / (top - bottom);
		res(3, 2) = -(zFar + zNear) / (zFar - zNear);
		return res;
	}

	Matrix4x4 Matrix4x4::OrthoImgui(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
	{
		Matrix4x4 res{ 1.0f };
		res(0, 0) = 2.0f / (right - left);
		res(1, 1) = 2.0f / (top - bottom);
		res(2, 2) = 1.0f / (zFar - zNear);
		res(3, 0) = (left + right) / (left - right);
		res(3, 1) = (bottom + top) / (bottom - top);
		res(3, 2) = zNear / (zNear - zFar);
		return res;
	}

	Matrix4x4 Matrix4x4::Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar)
	{
#ifndef NDEBUG
		assert(abs(aspect - std::numeric_limits<f32>::epsilon()) > 0.0f);
#endif
		f32 const halfTan = std::tanf(DegreeToRadian(fov) / 2.0f);
		Matrix4x4 res{ 0.0f };
		res(0, 0) = 1.0f / (aspect * halfTan);
		res(1, 1) = 1.0f / halfTan;
		res(2, 2) = -(zFar + zNear) / (zFar - zNear);
		res(2, 3) = -1.0f;
		res(3, 2) = -(2.0f * zFar * zNear) / (zFar - zNear);
		return res;
	}

	Matrix4x4 Matrix4x4::LookAt(Vector3 pos, Vector3 tgt, Vector3 up)
	{
		vec3 const w{ vec3::Normalize(pos - tgt) };				// front
		vec3 const u{ vec3::Normalize(vec3::Cross(up, w)) };	// right
		vec3 const v{ vec3::Normalize(vec3::Cross(w , u)) };	// up

		return mat4
		{
			vec4{ u.x, v.x, w.x, 0.0f },
			vec4{ u.y, v.y, w.y, 0.0f },
			vec4{ u.z, v.z, w.z, 0.0f },
			vec4{ -vec3::Dot(u, pos), -vec3::Dot(v, pos), -vec3::Dot(w, pos), 1.0f}
		};
	}

	Matrix4x4 Matrix4x4::Model(Vector3 const& pos, Vector3 const& scale, f32 rot)
	{
		f32 const rad = DegreeToRadian(rot);
		f32 const cos = std::cosf(rad), sin = std::sinf(rad);

		return Matrix4x4
		(
			Vector4{ scale.x * cos,  -(scale.y * sin),	0.0f,  pos.x },
			Vector4{ scale.x * sin,	   scale.y * cos,	0.0f,  pos.y },
			Vector4{ 0.0f,			   0.0f,			1.0f,  pos.z },
			Vector4{ 0.0f,			   0.0f,			0.0f,  1.0f }
		);
	}

	Matrix4x4 Matrix4x4::Model(Transform const& trans)
	{
		return Model(trans.localPosition, trans.localScale, trans.localRotation);
	}

	Matrix4x4 Matrix4x4::ModelT(Vector3 const& pos, Vector3 const& scale, f32 rot)
	{
		return Model(pos, scale, rot).Transpose();
	}

	Matrix4x4 Matrix4x4::ModelT(Transform const& trans)
	{
		return Model(trans).Transpose();
	}

	Matrix4x4 Matrix4x4::Inverse(Matrix4x4 const& mat)
	{
#ifndef NDEBUG
		f32 const det = Determinant(mat);
		assert(!Utility::IsEqual(det, 0.0f) && "Determinant is 0, unable to proceed due to division by 0!!");
		f32 const oneOverDeterminant = 1.0f / det;
#else
		f32 const oneOverDeterminant = 1.0f / Determinant(mat);
#endif
		mat4 adj{ 1.0f }; mat4 const transposed{ mat.Transpose() };
		vec4 const row0 = transposed(0), row1 = transposed(1), row2 = transposed(2), row3 = transposed(3);

		mat3 const mat00{  vec3{ row1.y, row1.z, row1.w },  vec3{ row2.y, row2.z, row2.w },  vec3{ row3.y, row3.z, row3.w } },
				   mat01{ -vec3{ row1.x, row1.z, row1.w }, -vec3{ row2.x, row2.z, row2.w }, -vec3{ row3.x, row3.z, row3.w } },
				   mat02{  vec3{ row1.x, row1.y, row1.w },  vec3{ row2.x, row2.y, row2.w },  vec3{ row3.x, row3.y, row3.w } },
				   mat03{ -vec3{ row1.x, row1.y, row1.z }, -vec3{ row2.x, row2.y, row2.z }, -vec3{ row3.x, row3.y, row3.z } };

		mat3 const mat10{ -vec3{ row0.y, row0.z, row0.w }, -vec3{ row2.y, row2.z, row2.w }, -vec3{ row3.y, row3.z, row3.w } },
				   mat11{  vec3{ row0.x, row0.z, row0.w },  vec3{ row2.x, row2.z, row2.w },  vec3{ row3.x, row3.z, row3.w } },
				   mat12{ -vec3{ row0.x, row0.y, row0.w }, -vec3{ row2.x, row2.y, row2.w }, -vec3{ row3.x, row3.y, row3.w } },
				   mat13{  vec3{ row0.x, row0.y, row0.z },  vec3{ row2.x, row2.y, row2.z },  vec3{ row3.x, row3.y, row3.z } };

		mat3 const mat20{  vec3{ row0.y, row0.z, row0.w },  vec3{ row1.y, row1.z, row1.w },  vec3{ row3.y, row3.z, row3.w } },
				   mat21{ -vec3{ row0.x, row0.z, row0.w }, -vec3{ row1.x, row1.z, row1.w }, -vec3{ row3.x, row3.z, row3.w } },
				   mat22{  vec3{ row0.x, row0.y, row0.w },  vec3{ row1.x, row1.y, row1.w },  vec3{ row3.x, row3.y, row3.w } },
				   mat23{ -vec3{ row0.x, row0.y, row0.z }, -vec3{ row1.x, row1.y, row1.z }, -vec3{ row3.x, row3.y, row3.z } };

		mat3 const mat30{ -vec3{ row0.y, row0.z, row0.w }, -vec3{ row1.y, row1.z, row1.w }, -vec3{ row2.y, row2.z, row2.w } },
				   mat31{  vec3{ row0.x, row0.z, row0.w },  vec3{ row1.x, row1.z, row1.w },  vec3{ row2.x, row2.z, row2.w } },
				   mat32{ -vec3{ row0.x, row0.y, row0.w }, -vec3{ row1.x, row1.y, row1.w }, -vec3{ row2.x, row2.y, row2.w } },
				   mat33{  vec3{ row0.x, row0.y, row0.z },  vec3{ row1.x, row1.y, row1.z },  vec3{ row2.x, row2.y, row2.z } };

		adj(0, 0) = mat00.Determinant(), adj(0, 1) = mat01.Determinant(), adj(0, 2) = mat02.Determinant(), adj(0, 3) = mat03.Determinant();
		adj(1, 0) = mat10.Determinant(), adj(1, 1) = mat11.Determinant(), adj(1, 2) = mat12.Determinant(), adj(1, 3) = mat13.Determinant();
		adj(2, 0) = mat20.Determinant(), adj(2, 1) = mat21.Determinant(), adj(2, 2) = mat22.Determinant(), adj(2, 3) = mat23.Determinant();
		adj(3, 0) = mat30.Determinant(), adj(3, 1) = mat31.Determinant(), adj(3, 2) = mat32.Determinant(), adj(3, 3) = mat33.Determinant();

		return oneOverDeterminant * adj;
	}

	Matrix4x4 Matrix4x4::InverseT(Matrix4x4 const& mat)
	{
		return Inverse(mat).Transpose();
	}

	Matrix4x4 Matrix4x4::Transpose(Matrix4x4 const& mat)
	{
		return mat4
		{
			vec4{ mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0) },
			vec4{ mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1) },
			vec4{ mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2) },
			vec4{ mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3) },
		};
	}

	f32 Matrix4x4::Determinant(Matrix4x4 const& mat)
	{
		vec4 const row0 = mat(0), row1 = mat(1), row2 = mat(2), row3 = mat(3);
		f32 const det0 = row0.x * mat3({ row1.y, row1.z, row1.w }, { row2.y, row2.z, row2.w }, { row3.y, row3.z, row3.w }).Determinant();
		f32 const det1 = row0.y * mat3({ row1.x, row1.z, row1.w }, { row2.x, row2.z, row2.w }, { row3.x, row3.z, row3.w }).Determinant();
		f32 const det2 = row0.z * mat3({ row1.x, row1.y, row1.w }, { row2.x, row2.y, row2.w }, { row3.x, row3.y, row3.w }).Determinant();
		f32 const det3 = row0.w * mat3({ row1.x, row1.y, row1.z }, { row2.x, row2.y, row2.z }, { row3.x, row3.y, row3.z }).Determinant();
		return (+det0) + (-det1) + (+det2) + (-det3);
	}

	Matrix4x4 operator+(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
	{
		Matrix4x4 res{ lhs }; res += rhs;
		return res;
	}

	Matrix4x4 operator-(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
	{
		Matrix4x4 res{ lhs }; res -= rhs;
		return res;
	}

	Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs)
	{
		Matrix4x4 res{ lhs }; res *= rhs;
		return res;
	}

	Matrix4x4 operator*(Matrix4x4 const& lhs, typename Matrix4x4::value_type rhs)
	{
		Matrix4x4 res{ lhs }; res *= rhs;
		return res;
	}

	Matrix4x4 operator*(typename Matrix4x4::value_type lhs, Matrix4x4 const& rhs)
	{
		Matrix4x4 res{ rhs }; res *= lhs;
		return res;
	}

	Vector3 operator*(Matrix4x4 const& lhs, Vector3 const& rhs)
	{
		f32 x{}, y{}, z{};
		x = lhs(0, 0) * rhs.x + lhs(0, 1) * rhs.y + lhs(0, 2) * rhs.z + lhs(0, 3);
		y = lhs(1, 0) * rhs.x + lhs(1, 1) * rhs.y + lhs(1, 2) * rhs.z + lhs(1, 3);
		z = lhs(2, 0) * rhs.x + lhs(2, 1) * rhs.y + lhs(2, 2) * rhs.z + lhs(2, 3);

		return Vector3{ x, y, z };
	}

	Vector4 operator*(Matrix4x4 const& lhs, Vector4 const& rhs)
	{
		f32 x{}, y{}, z{}, w{};
		x = lhs(0, 0) * rhs.x + lhs(0, 1) * rhs.y + lhs(0, 2) * rhs.z + lhs(0, 3) * rhs.w;
		y = lhs(1, 0) * rhs.x + lhs(1, 1) * rhs.y + lhs(1, 2) * rhs.z + lhs(1, 3) * rhs.w;
		z = lhs(2, 0) * rhs.x + lhs(2, 1) * rhs.y + lhs(2, 2) * rhs.z + lhs(2, 3) * rhs.w;
		w = lhs(3, 0) * rhs.x + lhs(3, 1) * rhs.y + lhs(3, 2) * rhs.z + lhs(3, 3) * rhs.w;

		return Vector4{ x, y, z, w };
	}

	std::ostream& operator<<(std::ostream& os, Matrix4x4 const& rhs)
	{
		os << std::fixed << std::setprecision(5);
		u64 constexpr MAX{ 4 };
		for (u64 i = 0; i < MAX; ++i)
		{
			for (u64 j = 0; j < MAX; ++j)
				os << std::left << std::setw(10) << rhs(i, j) << (j + 1 == MAX ? '\0' : ' ');
			os << std::endl;
		}
		return os;
	}
}