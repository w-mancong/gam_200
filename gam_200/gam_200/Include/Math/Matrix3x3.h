#ifndef	MATRIX_3X3
#define MATRIX_3X3

#include "matrix.h"
namespace ManCong
{
	namespace Math
	{
		class Vector3;
		class Matrix3x3 : public matrix
		{
		public:
			Matrix3x3(void);
			Matrix3x3(Vector3 f = Vector3(1.0f, 0.0f, 0.0f), Vector3 s = Vector3(0.0f, 1.0f, 0.0f), Vector3 t = Vector3(0.0f, 0.0f, 1.0f));
			virtual ~Matrix3x3(void);

			Matrix3x3& operator+=(Matrix3x3 const& rhs);
			Matrix3x3& operator-=(Matrix3x3 const& rhs);
			Matrix3x3& operator*=(Matrix3x3 const& rhs);
			Matrix3x3& operator*=(value_type rhs);

			Matrix3x3& Translate(float x, float y);
			Matrix3x3& Translate(Vector2 const& rhs);
			Matrix3x3& Scale(float x, float y);
			Matrix3x3& Scale(Vector2 const& rhs);
			Matrix3x3& RotationRad(float rad);
			Matrix3x3& RotationDeg(float deg);

		private:
			static size_type const R = 3, C = 3;
		};

		Matrix3x3 Mtx3x3Translate(float x, float y);
		Matrix3x3 Mtx3x3Translate(Vector2 const& rhs);
		Matrix3x3 Mtx3x3Scale(float x, float y);
		Matrix3x3 Mtx3x3Scale(Vector2 const& rhs);
		Matrix3x3 Mtx3x3RotRad(float rad);
		Matrix3x3 Mtx3x3RotDeg(float deg);

		Matrix3x3 operator+(Matrix3x3 const& lhs, Matrix3x3 const& rhs);
		Matrix3x3 operator-(Matrix3x3 const& lhs, Matrix3x3 const& rhs);
		Matrix3x3 operator*(Matrix3x3 const& lhs, Matrix3x3 const& rhs);

		Matrix3x3 operator*(Matrix3x3 const& lhs, typename Matrix3x3::value_type rhs);
		Matrix3x3 operator*(typename Matrix3x3::value_type lhs, Matrix3x3 const& rhs);

		Vector2 operator*(Matrix3x3 const& lhs, Vector2 const& rhs);
	}
}

#endif