#ifndef	MATRIX_4X4_H
#define MATRIX_4X4_H

#include "matrix.h"
namespace ManCong
{
	namespace Math
	{
		class Matrix4x4 : public matrix
		{
		public:
			Matrix4x4(value_type value);
			Matrix4x4(Vector4 r1 = Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4 r2 = Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4 r3 = Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4 r4 = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			virtual ~Matrix4x4(void) = default;

			Matrix4x4& operator+=(Matrix4x4 const& rhs);
			Matrix4x4& operator-=(Matrix4x4 const& rhs);
			Matrix4x4& operator*=(Matrix4x4 const& rhs);
			Matrix4x4& operator*=(value_type rhs);

			static Matrix4x4 Translate(f32 x, f32 y, f32 z);
			static Matrix4x4 Translate(Vector3 const& rhs);
			static Matrix4x4 Scale(f32 x, f32 y, f32 z);
			static Matrix4x4 Scale(Vector3 const& rhs);
			static Matrix4x4 Rotation(f32 angle, Vector3 const& axis);
			static Matrix4x4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
			static Matrix4x4 Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar);

		private:
			static size_type constexpr R = 4, C = 4;
		}; using Mat4 = Matrix4x4; using Mtx4 = Matrix4x4;

		Matrix4x4 operator+(Matrix4x4 const& lhs, Matrix4x4 const& rhs);
		Matrix4x4 operator-(Matrix4x4 const& lhs, Matrix4x4 const& rhs);
		Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs);

		Matrix4x4 operator*(Matrix4x4 const& lhs, typename Matrix4x4::value_type rhs);
		Matrix4x4 operator*(typename Matrix4x4::value_type lhs, Matrix4x4 const& rhs);

		Vector3 operator*(Matrix4x4 const& lhs, Vector3 const& rhs);
	}
}

#endif