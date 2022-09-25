/*!
file:	Vector2.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain class definition for representing a 2D vector

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	VECTOR2_H
#define VECTOR2_H

namespace ALEngine::Math
{
	class Vector3; class Vector4;
	class Vector2
	{
	public:
		f32 x, y;
	public:
		/*!*********************************************************************************
			\brief
			Constructor
		***********************************************************************************/
		Vector2(void);

		/*!*********************************************************************************
			\brief
			Constructor that takes in two floats for initalization

			\param [in] x:
			X component of the vector
			\param [in] y:
			Y component of the vector
		***********************************************************************************/
		Vector2(f32 x, f32 y);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector3 into a Vector2
		***********************************************************************************/
		Vector2(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector4 into a Vector2
		***********************************************************************************/
		Vector2(Vector4 const& rhs);

		/*!*********************************************************************************
			\brief
			Desturctor
		***********************************************************************************/
		~Vector2(void) = default;

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector3 into a Vector2
		***********************************************************************************/
		Vector2& operator=(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector4 into a Vector2
		***********************************************************************************/
		Vector2& operator=(Vector4 const& rhs);

		/*********************************************************************************
											MATH ARITHMETIC
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Add two 2D vectors together

			\param [in] rhs:
			2D vector on the right hand side to be added together with

			\return
			A reference to this vector after the summation of the two vectors
		***********************************************************************************/
		Vector2& operator+=(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract two 2D vectors from each other

			\param [in] rhs:
			2D vector on the right hand side to be subtracted from

			\return
			A reference to this vector after subtracting the two vectors
		***********************************************************************************/
		Vector2& operator-=(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar multiplication with 2D vector

			\param [in] rhs:
			Scalar value to be multiplied with

			\return
			A reference to this vector after multiplying the vector with the scalar
		***********************************************************************************/
		Vector2& operator*=(f32 rhs);

		/*!*********************************************************************************
			\brief
			Scalar division with 2D vector

			\param [in] rhs:
			Scalar value to be divided with

			\return
			A reference to this vector after dividing the vector by the scalar
		***********************************************************************************/
		Vector2& operator/=(f32 rhs);

		/*!*********************************************************************************
			\brief
			Negates the value of this vector

			\return
			A copy of this vector after negating the values
		***********************************************************************************/
		Vector2 operator-(void) const;

		/*!*********************************************************************************
			\brief
			Dot product with another vector

			\param [in] rhs:
			Vector to be dotted with to produce the dot product

			\return
			The dot product between this vector and rhs
		***********************************************************************************/
		f32 Dot(Vector2 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Calculates the magnitude of this vector

			\return
			The magnitude of this vector
		***********************************************************************************/
		f32 Magnitude(void) const;

		/*!*********************************************************************************
			\brief
			Calculates the magnitude square of this vector

			\return
			The squared magnitude of this vector
		***********************************************************************************/
		f32 MagnitudeSq(void) const;

		/*!*********************************************************************************
			\brief
			Normalize this vector
		***********************************************************************************/
		void Normalized(void);

		/*!*********************************************************************************
			\brief
			Normalize a copy of this vector

			\return
			The copy of this vector in it's normalized form
		***********************************************************************************/
		Vector2 Normalize(void) const;

		/*********************************************************************************
												UTILITIES
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Check if the two vectors are the same

			\param [in] rhs:
			Vector on the right hand side to be checked with

			\return 
			True if this vector is the same as rhs, else false
		***********************************************************************************/
		bool operator==(Vector2 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Check if the two vectors are NOT the same

			\param [in] rhs:
			Vector on the right hand side to be checked with

			\return
			True if this vector is NOT the same as rhs, else false
		***********************************************************************************/
		bool operator!=(Vector2 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Set this vector's x and y component to zero
		***********************************************************************************/
		void SetZero(void);

		/*!*********************************************************************************
			\brief
			Check if this vector's x and y component are zero

			\return
			True if both x and y are zero, else false
		***********************************************************************************/
		bool IsZero(void) const;

		/*********************************************************************************
										STATIC FUNCTIONS
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Find the angle between two vectors

			\param [in] from:
			Angle from this vector (1st vector)
			\param [in] to:
			To the angle of this vector (2nd vector)

			\return
			Angle in degrees between the two vectors
		***********************************************************************************/
		static f32 Angle(Vector2 const& from, Vector2 const& to);

		/*!*********************************************************************************
			\brief
			Clamp the magnitude of lhs to the maxLength
				
			\param [in] lhs:
			To return a copy of this vector with it's magnitude clamp with maxLength

			\return
			A copy of lhs with magnitude maxLength
		***********************************************************************************/
		static Vector2 ClampMagnitude(Vector2 const& lhs, f32 maxLength);

		/*!*********************************************************************************
			\brief
			Find the distance between the two vectors

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			Distance between the two vectors
		***********************************************************************************/
		static f32 Distance(Vector2 const& lhs, Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Find the squared distance between the two vectors

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			Square distance between the two vectors
		***********************************************************************************/
		static f32 DistanceSq(Vector2 const& lhs, Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			The dot product between the two vectors

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			The dot product between the two vectors
		***********************************************************************************/
		static f32 Dot(Vector2 const& lhs, Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Create a copy of a 2D vector with the maximum value from lhs and rhs

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			A copy of 2D vector with the maximum values from the two vectors
		***********************************************************************************/
		static Vector2 Max(Vector2 const& lhs, Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Create a copy of a 2D vector with the minimum value from lhs and rhs

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			A copy of 2D vector with the minimum values from the two vectors
		***********************************************************************************/
		static Vector2 Min(Vector2 const& lhs, Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Create a copy of a 2D vector from inDirection that is perpendicular to it. The
			vector will always be 90-degrees anti-clockwise direction

			\param [in] inDirection
			The vector to create a penpendicular vector from

			\return
			A vector that is 90-degrees anti-clockwise direction from inDirection
		***********************************************************************************/
		static Vector2 Perpendicular(Vector2 const& inDirection);

		/*!*********************************************************************************
			\brief

		***********************************************************************************/
		//static Vector2 Reflect(Vector2 const& inDirection, Vector2 const& inNormal);

		/*!*********************************************************************************
			\brief
			Normalize the vector rhs

			\param [in] rhs:
			The vector to be normalize

			\return
			A copy of rhs in it's normalize form
		***********************************************************************************/
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
		/*!*********************************************************************************
			\brief
			Swap the data of this vector with rhs

			\param [in, out] rhs:
			Vector to have it's data swapped with
		***********************************************************************************/
		void swap(Vector2& rhs);
	}; using Vec2 = Vector2; using Vec2D = Vector2; using vec2 = Vector2;

	/*!*********************************************************************************
		\brief
		Overloaded + operator to add two 2D vectors together

		\param [in] lhs:
		Vector on the left hand side of the + operator to be added with
		\param [in] rhs:
		Vector on the right hand side of the + operator to be added with

		\return
		2D vector containing the summation of lhs and rhs
	***********************************************************************************/
	Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded - operator to subtract two 2D vectors from each other

		\param [in] lhs:
		Vector on the left hand side of the - operator to be subtracted from
		\param [in] rhs:
		Vector on the right hand side of the - operator to be subtracted from

		\return 
		2D vector containing the subtraction of lhs and rhs
	***********************************************************************************/
	Vector2 operator-(Vector2 const& lhs, Vector2 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded * operator to multiply a vector with a scalar

		\param [in] lhs:
		Vector on the left hand side of the * operator to be multiplied with
		\param [in] rhs:
		Scalar value on the right hand side of the * operator to be multiplied with

		\return
		2D vector containing the multiplication of vector lhs and scalar value rhs
	***********************************************************************************/
	Vector2 operator*(Vector2 const& lhs, f32 rhs);

	/*!*********************************************************************************
		\brief
		Overloaded * operator to multiply a vector with a scalar

		\param [in] lhs:
		Scalar value on the left hand side of the * operator to be multiplied with
		\param [in] rhs:
		Vector on the right hand side of the * operator to be multiplied with

		\return
		2D vector containing the multiplcaition of scalar value lhs and vector rhs
	***********************************************************************************/
	Vector2 operator*(f32 lhs, Vector2 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded / operator to divide a vector by scalar

		\param [in] lhs:
		Vector on the right hand side of the / operator to be divided with
		\param [in] rhs:
		Scalar value on the right hand side of the / operator to divide the vector by

		\return
		2D vector containing the division of vector lhs by scalar value rhs
	***********************************************************************************/
	Vector2 operator/(Vector2 const& lhs, f32 rhs);

	/*!*********************************************************************************
		\brief
		To output the data of Vector2 to the output stream

		\param [in] os:
		Reference to the output stream to stream the Vector2 data to
		\param [in] rhs:
		Vector data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vector2 const& rhs);
}

#endif