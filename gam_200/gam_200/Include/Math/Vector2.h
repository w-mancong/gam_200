#ifndef	VECTOR2_H
#define VECTOR2_H

#include <iostream>
namespace ManCong
{
	namespace Math
	{
		class Vector3;
		class Vector2
		{
		public:
			Vector2(void);
			Vector2(float x, float y);
			Vector2(Vector3 const& rhs);
			~Vector2(void);

			Vector2& operator=(Vector3 const& rhs);

			/*********************************************************************************
												MATH ARITHMETIC
			*********************************************************************************/
			Vector2& operator+=(Vector2 const& rhs);
			Vector2& operator-=(Vector2 const& rhs);
			Vector2& operator*=(float rhs);
			Vector2& operator/=(float rhs);
			Vector2 operator-(void) const;

			float Dot(Vector2 const& rhs) const;
			float Magnitude(void) const;
			float MagnitudeSq(void) const;
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
			static float Angle(Vector2 const& from, Vector2 const& to);
			static Vector2 ClampMagnitude(Vector2 const& lhs, float maxLength);
			static float Distance(Vector2 const& lhs, Vector2 const& rhs);
			static float DistanceSq(Vector2 const& lhs, Vector2 const& rhs);
			static float Dot(Vector2 const& lhs, Vector2 const& rhs);
			static Vector2 Max(Vector2 const& lhs, Vector2 const& rhs);
			static Vector2 Min(Vector2 const& lhs, Vector2 const& rhs);
			static Vector2 Perpendicular(Vector2 const& inDirection);
			//static Vector2 Reflect(Vector2 const& inDirection, Vector2 const& inNormal);
			static Vector2 Normalize(Vector2 const& rhs);

			float x, y;

			static Vector2 const up;
			static Vector2 const down;
			static Vector2 const left;
			static Vector2 const right;
			static Vector2 const one;
			static Vector2 const zero;
			static Vector2 const positiveInfinity;
			static Vector2 const negativeInfinity;
		};

		Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs);
		Vector2 operator-(Vector2 const& lhs, Vector2 const& rhs);
		Vector2 operator*(Vector2 const& lhs, float rhs);
		Vector2 operator*(float lhs, Vector2 const& rhs);
		Vector2 operator/(Vector2 const& lhs, float rhs);

		std::ostream& operator<<(std::ostream& os, Vector2 const& rhs);
	}
}

#endif