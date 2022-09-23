/*!
file:	Vector4.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain class definition for representing a 4D vector

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	VECTOR4_H
#define VECTOR4_H

#include <iostream>
#include "Utility/Type.h"
namespace ALEngine::Math
{
	class Vector2; class Vector3;
	class Vector4
	{
	public:
		f32 x, y, z, w;
	public:
		/*!*********************************************************************************
			\brief
			Constructor
		***********************************************************************************/
		Vector4(void);

		/*!*********************************************************************************
			\brief
			Constructor that takes in two floats for initalization

			\param [in] x:
			X component of the vector
			\param [in] y:
			Y component of the vector
			\param [in] z:
			Z component of the vector
			\param [in] w:
			W component of the vector
		***********************************************************************************/
		Vector4(f32 x, f32 y, f32 z, f32 w);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector2 into a Vector4
		***********************************************************************************/
		Vector4(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector3 into a Vector4
		***********************************************************************************/
		Vector4(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Desturctor
		***********************************************************************************/
		~Vector4(void) = default;

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector2 into a Vector4
		***********************************************************************************/
		Vector4& operator=(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector3 into a Vector4
		***********************************************************************************/
		Vector4& operator=(Vector3 const& rhs);

		/*********************************************************************************
											MATH ARITHMETIC
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Add two 4D vectors together

			\param [in] rhs:
			4D vector on the right hand side to be added together with

			\return
			A reference to this vector after the summation of the two vectors
		***********************************************************************************/
		Vector4& operator+=(Vector4 const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract two 4D vectors from each other

			\param [in] rhs:
			4D vector on the right hand side to be subtracted from

			\return
			A reference to this vector after subtracting the two vectors
		***********************************************************************************/
		Vector4& operator-=(Vector4 const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar multiplication with 4D vector

			\param [in] rhs:
			Scalar value to be multiplied with

			\return
			A reference to this vector after multiplying the vector with the scalar
		***********************************************************************************/
		Vector4& operator*=(f32 rhs);

		/*!*********************************************************************************
			\brief
			Scalar division with 4D vector

			\param [in] rhs:
			Scalar value to be divided with

			\return
			A reference to this vector after dividing the vector by the scalar
		***********************************************************************************/
		Vector4& operator/=(f32 rhs);

		/*!*********************************************************************************
			\brief
			Negates the value of this vector

			\return
			A copy of this vector after negating the values
		***********************************************************************************/
		Vector4 operator-(void) const;

		/*!*********************************************************************************
			\brief
			Dot product with another vector

			\param [in] rhs:
			Vector to be dotted with to produce the dot product

			\return
			The dot product between this vector and rhs
		***********************************************************************************/
		f32 Dot(Vector4 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Calculates the magnitude of this vector

			\return
			The magnitude of this vector
		***********************************************************************************/
		f32 Length(void) const;

		/*!*********************************************************************************
			\brief
			Calculates the magnitude square of this vector

			\return
			The squared magnitude of this vector
		***********************************************************************************/
		f32 LengthSq(void) const;

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
		Vector4 Normalize(void) const;

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
		bool operator==(Vector4 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Check if the two vectors are NOT the same

			\param [in] rhs:
			Vector on the right hand side to be checked with

			\return
			True if this vector is NOT the same as rhs, else false
		***********************************************************************************/
		bool operator!=(Vector4 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Set this vector's x, y, z and w component to zero
		***********************************************************************************/
		void SetZero(void);

		/*!*********************************************************************************
			\brief
			Check if this vector's x, y, z and w component are zero

			\return
			True if both x, y, z and w are zero, else false
		***********************************************************************************/
		bool IsZero(void) const;

	private:
		/*!*********************************************************************************
			\brief
			Swap the data of this vector with rhs

			\param [in, out] rhs:
			Vector to have it's data swapped with
		***********************************************************************************/
		void swap(Vector4& rhs);
	}; using Vec4 = Vector4; using Vec4D = Vector4; using vec4 = Vector4;

	/*!*********************************************************************************
		\brief
		Overloaded + operator to add two 4D vectors together

		\param [in] lhs:
		Vector on the left hand side of the + operator to be added with
		\param [in] rhs:
		Vector on the right hand side of the + operator to be added with

		\return
		4D vector containing the summation of lhs and rhs
	***********************************************************************************/
	Vector4 operator+(Vector4 const& lhs, Vector4 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded - operator to subtract two 4D vectors from each other

		\param [in] lhs:
		Vector on the left hand side of the - operator to be subtracted from
		\param [in] rhs:
		Vector on the right hand side of the - operator to be subtracted from

		\return
		4D vector containing the subtraction of lhs and rhs
	***********************************************************************************/
	Vector4 operator-(Vector4 const& lhs, Vector4 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded * operator to multiply a vector with a scalar

		\param [in] lhs:
		Vector on the left hand side of the * operator to be multiplied with
		\param [in] rhs:
		Scalar value on the right hand side of the * operator to be multiplied with

		\return
		4D vector containing the multiplication of vector lhs and scalar value rhs
	***********************************************************************************/
	Vector4 operator*(Vector4 const& lhs, f32 rhs);

	/*!*********************************************************************************
		\brief
		Overloaded * operator to multiply a vector with a scalar

		\param [in] lhs:
		Scalar value on the left hand side of the * operator to be multiplied with
		\param [in] rhs:
		Vector on the right hand side of the * operator to be multiplied with

		\return
		4D vector containing the multiplcaition of scalar value lhs and vector rhs
	***********************************************************************************/
	Vector4 operator*(f32 lhs, Vector4 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded / operator to divide a vector by scalar

		\param [in] lhs:
		Vector on the right hand side of the / operator to be divided with
		\param [in] rhs:
		Scalar value on the right hand side of the / operator to divide the vector by

		\return
		4D vector containing the division of vector lhs by scalar value rhs
	***********************************************************************************/
	Vector4 operator/(Vector4 const& lhs, f32 rhs);

	/*!*********************************************************************************
		\brief
		To output the data of Vector3 to the output stream

		\param [in] os:
		Reference to the output stream to stream the Vector3 data to
		\param [in] rhs:
		Vector data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vector4 const& rhs);
}

#endif