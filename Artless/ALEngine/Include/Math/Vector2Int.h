#ifndef	VECTOR2INT_H
#define VECTOR2INT_H

/*!
file:	Vector2Int.h
author: Chan Jie Ming Stanley
email:	c.jiemingstanley\@digipen.edu
brief:	This file contain class definition for representing a 2D vector and points using 
integers where the precision of floating-point is not required

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/

namespace ALEngine::Math
{
	class Vector3Int;
	class Vector2Int
	{
	public:
		s32 x, y;
	public:
		/*!*********************************************************************************
			\brief
			Constructor
		***********************************************************************************/
		Vector2Int(void);

		/*!*********************************************************************************
			\brief
			Constructor that takes in two int for initalization

			\param [in] x:
			X component of the vector
			\param [in] y:
			Y component of the vector
		***********************************************************************************/
		Vector2Int(s32 x, s32 y);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector3 into a Vector2
		***********************************************************************************/
		Vector2Int(Vector3Int const& rhs);

		/*!*********************************************************************************
			\brief
			Desturctor
		***********************************************************************************/
		~Vector2Int(void) = default;

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector3 into a Vector2
		***********************************************************************************/
		Vector2Int& operator=(Vector3Int const& rhs);

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
		Vector2Int& operator+=(Vector2Int const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract two 2D vectors from each other

			\param [in] rhs:
			2D vector on the right hand side to be subtracted from

			\return
			A reference to this vector after subtracting the two vectors
		***********************************************************************************/
		Vector2Int& operator-=(Vector2Int const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar multiplication with 2D vector

			\param [in] rhs:
			Scalar value to be multiplied with

			\return
			A reference to this vector after multiplying the vector with the scalar
		***********************************************************************************/
		Vector2Int& operator*=(s32 rhs);

		/*!*********************************************************************************
			\brief
			Scalar division with 2D vector

			\param [in] rhs:
			Scalar value to be divided with

			\return
			A reference to this vector after dividing the vector by the scalar
		***********************************************************************************/
		Vector2Int& operator/=(s32 rhs);

		/*!*********************************************************************************
			\brief
			Negates the value of this vector

			\return
			A copy of this vector after negating the values
		***********************************************************************************/
		Vector2Int operator-(void) const;

		/*!*********************************************************************************
			\brief
			Dot product with another vector

			\param [in] rhs:
			Vector to be dotted with to produce the dot product

			\return
			The dot product between this vector and rhs
		***********************************************************************************/
		s32 Dot(Vector2Int const& rhs) const;

		/*!*********************************************************************************
			\brief
			Calculates the magnitude of this vector

			\return
			The magnitude of this vector
		***********************************************************************************/
		s32 Magnitude(void) const;

		/*!*********************************************************************************
			\brief
			Calculates the magnitude square of this vector

			\return
			The squared magnitude of this vector
		***********************************************************************************/
		s32 MagnitudeSq(void) const;

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
		bool operator==(Vector2Int const& rhs) const;

		/*!*********************************************************************************
			\brief
			Check if the two vectors are NOT the same

			\param [in] rhs:
			Vector on the right hand side to be checked with

			\return
			True if this vector is NOT the same as rhs, else false
		***********************************************************************************/
		bool operator!=(Vector2Int const& rhs) const;

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
			Find the distance between the two vectors

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			Distance between the two vectors
		***********************************************************************************/
		static s32 Distance(Vector2Int const& lhs, Vector2Int const& rhs);

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
		static s32 DistanceSq(Vector2Int const& lhs, Vector2Int const& rhs);

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
		static s32 Dot(Vector2Int const& lhs, Vector2Int const& rhs);

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
		static Vector2Int Max(Vector2Int const& lhs, Vector2Int const& rhs);

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
		static Vector2Int Min(Vector2Int const& lhs, Vector2Int const& rhs);

		static Vector2Int const up;
		static Vector2Int const down;
		static Vector2Int const left;
		static Vector2Int const right;
		static Vector2Int const one;
		static Vector2Int const zero;
	
	private:
		/*!*********************************************************************************
			\brief
			Swap the data of this vector with rhs

			\param [in, out] rhs:
			Vector to have it's data swapped with
		***********************************************************************************/
		void swap(Vector2Int& rhs);
	}; using Vec2Int = Vector2Int; using Vec2DInt = Vector2Int; using vec2Int = Vector2Int;

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
	Vector2Int operator+(Vector2Int const& lhs, Vector2Int const& rhs);

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
	Vector2Int operator-(Vector2Int const& lhs, Vector2Int const& rhs);

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
	Vector2Int operator*(Vector2Int const& lhs, s32 rhs);

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
	Vector2Int operator*(s32 lhs, Vector2Int const& rhs);

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
	Vector2Int operator/(Vector2Int const& lhs, s32 rhs);

	/*!*********************************************************************************
		\brief
		To output the data of Vector2 to the output stream

		\param [in] os:
		Reference to the output stream to stream the Vector2 data to
		\param [in] rhs:
		Vector data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vector2Int const& rhs);

}
#endif //!VECTOR2INT_H
