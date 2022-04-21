#ifndef	MATRIX_3X3_H
#define MATRIX_3X3_H

#include "matrix.h"
namespace ManCong
{
	namespace Math
	{
		class Matrix3x3 : public matrix
		{
		public:
			Matrix3x3(value_type value);
			Matrix3x3(Vector3 r1 = Vector3(1.0f, 0.0f, 0.0f), Vector3 r2 = Vector3(0.0f, 1.0f, 0.0f), Vector3 r3 = Vector3(0.0f, 0.0f, 1.0f));
			virtual ~Matrix3x3(void) = default;

			Matrix3x3& operator+=(Matrix3x3 const& rhs);
			Matrix3x3& operator-=(Matrix3x3 const& rhs);
			Matrix3x3& operator*=(Matrix3x3 const& rhs);
			Matrix3x3& operator*=(value_type rhs);

			static Matrix3x3 Translate(f32 x, f32 y);
			static Matrix3x3 Translate(Vector2 const& rhs);
			static Matrix3x3 Scale(f32 x, f32 y);
			static Matrix3x3 Scale(Vector2 const& rhs);
			static Matrix3x3 Rotation(f32 deg);

		private:
			static size_type constexpr R = 3, C = 3;
		}; using Mat3 = Matrix3x3; using Mtx3 = Matrix3x3;

		Matrix3x3 operator+(Matrix3x3 const& lhs, Matrix3x3 const& rhs);
		Matrix3x3 operator-(Matrix3x3 const& lhs, Matrix3x3 const& rhs);
		Matrix3x3 operator*(Matrix3x3 const& lhs, Matrix3x3 const& rhs);

		Matrix3x3 operator*(Matrix3x3 const& lhs, typename Matrix3x3::value_type rhs);
		Matrix3x3 operator*(typename Matrix3x3::value_type lhs, Matrix3x3 const& rhs);

		Vector2 operator*(Matrix3x3 const& lhs, Vector2 const& rhs);
	}
}

#endif