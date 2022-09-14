#include "pch.h"

namespace ALEngine
{
	namespace Math
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
			if (row >= 4 || row < 0 || col >= 4 || col < 0)
			{
				std::cerr << "Either row or column index is larger than 4 or smaller than 0" << std::endl;
			}

			return *(value_ptr() + row * 4 + col);
		}

		Matrix4x4& Matrix4x4::operator+=(Matrix4x4 const& rhs)
		{
			Matrix4x4 res(1.0f);  Matrix4 const& lhs = *this;
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
			Matrix4x4 res(1.0f);  Matrix4 const& lhs = *this;
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
			mat[0].x *= rhs; mat[0].y *= rhs; mat[0].z *= rhs; mat[0].w = rhs;
			mat[1].x *= rhs; mat[1].y *= rhs; mat[1].z *= rhs; mat[1].w = rhs;
			mat[2].x *= rhs; mat[2].y *= rhs; mat[2].z *= rhs; mat[2].w = rhs;
			mat[3].x *= rhs; mat[3].y *= rhs; mat[3].z *= rhs; mat[3].w = rhs;
			return *this;
		}

		typename Matrix4x4::const_pointer Matrix4x4::value_ptr(void) const
		{
			return &mat[0].x;
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

		Matrix4x4 Matrix4x4::Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar)
		{
#if _DEBUG
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

		Matrix4x4 Matrix4x4::LookAt(Vector3 eye, Vector3 center, Vector3 up)
		{
			// direction										// right							   // up
			Vector3 const d = Vector3::Normalize(center - eye), r = Vector3::Cross(d, up).Normalize(), u = Vector3::Cross(r, d);
			Matrix4x4 res;
			res(0, 0) = r.x;				   res(0, 1) = u.x;					  res(0, 2) = -d.x;
			res(1, 0) = r.y;				   res(1, 1) = u.y;					  res(1, 2) = -d.y;
			res(2, 0) = r.z;				   res(2, 1) = u.z;					  res(2, 2) = -d.z;
			res(3, 0) = -Vector3::Dot(r, eye); res(3, 1) = -Vector3::Dot(u, eye); res(3, 2) = Vector3::Dot(d, eye);
			return res;
		}

		Matrix4x4 Matrix4x4::Model(Vector3 const& pos, Vector3 const& scale, f32 rot)
		{
			f32 const rad = DegreeToRadian(rot);
			f32 const cos = std::cosf(rad), sin = std::sinf(rad);

			return Matrix4x4
			(
				Vector4{ scale.x * cos,  scale.x * sin,	0.0f,  0.0f },
				Vector4{ -(scale.y * sin), scale.y * cos,	0.0f,  0.0f },
				Vector4{ 0.0f,		   0.0f,			1.0f,  0.0f },
				Vector4{ pos.x,		   pos.y,			pos.z, 1.0f }
			);
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
			for (u64 i = 0; i < 4; ++i)
			{
				for (u64 j = 0; j < 4; ++j)
					os << std::left << std::setw(10) << rhs(i, j) << (j + 1 == 4 ? '\0' : ' ');
				os << std::endl;
			}
			return os;
		}
	}
}