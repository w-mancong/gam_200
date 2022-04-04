#ifndef	VECTOR3_H
#define VECTOR3_H

#include <iostream>
namespace ManCong
{
	namespace Math
	{
		class Vector2;
		class Vector3
		{
		public:
			Vector3(void);
			Vector3(float x, float y, float z);
			Vector3(Vector2 const& rhs);
			~Vector3(void);

			Vector3& operator=(Vector2 const& rhs);

			/*********************************************************************************
												MATH ARITHMETIC
			*********************************************************************************/
			Vector3& operator+=(Vector3 const& rhs);
			Vector3& operator-=(Vector3 const& rhs);
			Vector3& operator*=(float rhs);
			Vector3& operator/=(float rhs);
			Vector3 operator-(void) const;

			float Dot(Vector3 const& rhs) const;
			float Length(void) const;
			float LengthSq(void) const;
			void Normalized(void);
			Vector3 Normalize(void) const;

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

			float x, y, z;
		};

		Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs);
		Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs);
		Vector3 operator*(Vector3 const& lhs, float rhs);
		Vector3 operator*(float lhs, Vector3 const& rhs);
		Vector3 operator/(Vector3 const& lhs, float rhs);

		float Vector3Distance(Vector3 const& lhs, Vector3 const& rhs);
		float Vector3SquareDistance(Vector3 const& lhs, Vector3 const& rhs);

		std::ostream& operator<<(std::ostream& os, Vector3 const& rhs);
	}
}

#endif