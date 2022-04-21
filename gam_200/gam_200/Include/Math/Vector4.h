#ifndef	VECTOR4_H
#define VECTOR4_H

#include <iostream>
#include "Utility/Type.h"
namespace ManCong
{
	namespace Math
	{
		class Vector2; class Vector3;
		class Vector4
		{
		public:
			f32 x, y, z, w;
		public:
			Vector4(void);
			Vector4(f32 x, f32 y, f32 z, f32 w);
			Vector4(Vector2 const& rhs);
			Vector4(Vector3 const& rhs);
			~Vector4(void) = default;

			Vector4& operator=(Vector2 const& rhs);
			Vector4& operator=(Vector3 const& rhs);

			/*********************************************************************************
												MATH ARITHMETIC
			*********************************************************************************/
			Vector4& operator+=(Vector4 const& rhs);
			Vector4& operator-=(Vector4 const& rhs);
			Vector4& operator*=(f32 rhs);
			Vector4& operator/=(f32 rhs);
			Vector4 operator-(void) const;

			f32 Dot(Vector4 const& rhs) const;
			f32 Length(void) const;
			f32 LengthSq(void) const;
			void Normalized(void);
			Vector4 Normalize(void) const;

			/*********************************************************************************
												UTILITIES
			*********************************************************************************/
			bool operator==(Vector4 const& rhs) const;
			bool operator!=(Vector4 const& rhs) const;

			void SetZero(void);
			bool IsZero(void) const;

		private:
			void swap(Vector4& rhs);
		}; using Vec4 = Vector4; using Vec4D = Vector4;

		Vector4 operator+(Vector4 const& lhs, Vector4 const& rhs);
		Vector4 operator-(Vector4 const& lhs, Vector4 const& rhs);
		Vector4 operator*(Vector4 const& lhs, f32 rhs);
		Vector4 operator*(f32 lhs, Vector4 const& rhs);
		Vector4 operator/(Vector4 const& lhs, f32 rhs);

		f32 Vector4Distance(Vector4 const& lhs, Vector4 const& rhs);
		f32 Vector4SquareDistance(Vector4 const& lhs, Vector4 const& rhs);

		std::ostream& operator<<(std::ostream& os, Vector4 const& rhs);
	}
}

#endif