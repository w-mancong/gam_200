#ifndef	VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include "Utility/Type.h"
namespace ManCong
{
	namespace Math
	{
		class Vector3; class Vector4;
		class Vector2
		{
		public:
			f32 x, y;
		public:
			Vector2(void);
			Vector2(f32 x, f32 y);
			Vector2(Vector3 const& rhs);
			Vector2(Vector4 const& rhs);
			~Vector2(void) = default;

			Vector2& operator=(Vector3 const& rhs);
			Vector2& operator=(Vector4 const& rhs);

			/*********************************************************************************
												MATH ARITHMETIC
			*********************************************************************************/
			Vector2& operator+=(Vector2 const& rhs);
			Vector2& operator-=(Vector2 const& rhs);
			Vector2& operator*=(f32 rhs);
			Vector2& operator/=(f32 rhs);
			Vector2 operator-(void) const;

			f32 Dot(Vector2 const& rhs) const;
			f32 Magnitude(void) const;
			f32 MagnitudeSq(void) const;
			void Normalized(void);
			Vector2 Normalize(void) const;

			/*********************************************************************************
												  UTILITIES
			*********************************************************************************/
			bool operator==(Vector2 const& rhs) const;
			bool operator!=(Vector2 const& rhs) const;

			void SetZero(void);
			bool IsZero(void) const;

			/*********************************************************************************
											STATIC FUNCTIONS
			*********************************************************************************/
			static f32 Angle(Vector2 const& from, Vector2 const& to);
			static Vector2 ClampMagnitude(Vector2 const& lhs, f32 maxLength);
			static f32 Distance(Vector2 const& lhs, Vector2 const& rhs);
			static f32 DistanceSq(Vector2 const& lhs, Vector2 const& rhs);
			static f32 Dot(Vector2 const& lhs, Vector2 const& rhs);
			static Vector2 Max(Vector2 const& lhs, Vector2 const& rhs);
			static Vector2 Min(Vector2 const& lhs, Vector2 const& rhs);
			static Vector2 Perpendicular(Vector2 const& inDirection);
			//static Vector2 Reflect(Vector2 const& inDirection, Vector2 const& inNormal);
			static Vector2 Normalize(Vector2 const& rhs);

			static Vector2 const up;
			static Vector2 const down;
			static Vector2 const left;
			static Vector2 const right;
			static Vector2 const one;
			static Vector2 const zero;
			static Vector2 const positiveInfinity;
			static Vector2 const negativeInfinity;
		private:
			void swap(Vector2& rhs);
		}; using Vec2 = Vector2; using Vec2D = Vector2;

		Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs);
		Vector2 operator-(Vector2 const& lhs, Vector2 const& rhs);
		Vector2 operator*(Vector2 const& lhs, f32 rhs);
		Vector2 operator*(f32 lhs, Vector2 const& rhs);
		Vector2 operator/(Vector2 const& lhs, f32 rhs);

		std::ostream& operator<<(std::ostream& os, Vector2 const& rhs);
	}
}

#endif