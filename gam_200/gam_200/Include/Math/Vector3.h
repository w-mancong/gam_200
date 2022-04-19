#ifndef	VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include "Utility/Type.h"
namespace ManCong
{
	namespace Math
	{
		class Vector2; class Vector4;
		class Vector3
		{
		public:
			f32 x, y, z;
		public:
			Vector3(void);
			Vector3(f32 x, f32 y, f32 z);
			Vector3(Vector2 const& rhs);
			Vector3(Vector4 const& rhs);
			~Vector3(void) = default;

			Vector3& operator=(Vector2 const& rhs);
			Vector3& operator=(Vector4 const& rhs);

			/*********************************************************************************
												MATH ARITHMETIC
			*********************************************************************************/
			Vector3& operator+=(Vector3 const& rhs);
			Vector3& operator-=(Vector3 const& rhs);
			Vector3& operator*=(f32 rhs);
			Vector3& operator/=(f32 rhs);
			Vector3 operator-(void) const;

			f32 Dot(Vector3 const& rhs) const;
			f32 Length(void) const;
			f32 LengthSq(void) const;
			void Normalized(void);
			Vector3 Normalize(void) const;
			// Cross product function here

			/*********************************************************************************
												  UTILITIES
			*********************************************************************************/
			bool operator==(Vector3 const& rhs) const;
			bool operator!=(Vector3 const& rhs) const;

			void SetZero(void);
			bool IsZero(void) const;

			/*********************************************************************************
											  STATIC FUNCTIONS
			*********************************************************************************/
			static Vector3 const right;
			static Vector3 const left;
			static Vector3 const up;
			static Vector3 const down;
			static Vector3 const forward;
			static Vector3 const back;
			static Vector3 const one;
			static Vector3 const zero;
			static Vector3 const positiveInfinity;
			static Vector3 const negativeInfinity;

			// cross product
		private:
			void swap(Vector3& rhs);
		};

		Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs);
		Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs);
		Vector3 operator*(Vector3 const& lhs, f32 rhs);
		Vector3 operator*(f32 lhs, Vector3 const& rhs);
		Vector3 operator/(Vector3 const& lhs, f32 rhs);

		f32 Vector3Distance(Vector3 const& lhs, Vector3 const& rhs);
		f32 Vector3SquareDistance(Vector3 const& lhs, Vector3 const& rhs);

		std::ostream& operator<<(std::ostream& os, Vector3 const& rhs);
	}
}

#endif