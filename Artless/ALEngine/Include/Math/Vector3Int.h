#ifndef	VECTOR3INT_H
#define VECTOR3INT_H

/*!
file:	Vector3Int.h
author: Chan Jie Ming Stanley
email:	c.jiemingstanley\@digipen.edu
brief:	This file contain class definition for representing a 3D vector and points using
integers where the precision of floating-point is not required
All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
namespace ALEngine::Math
{
	class Vector2Int;
	class Vector3Int
	{
	public:
		s32 x, y, z;
	public:
		/*!*********************************************************************************
			\brief
			Constructor
		***********************************************************************************/
		Vector3Int(void);

		/*!*********************************************************************************
			\brief
			Constructor that takes in three ints for initalization

			\param [in] x:
			X component of the vector
			\param [in] y:
			Y component of the vector
			\param [in] z:
			Z component of the vector
		***********************************************************************************/
		Vector3Int(s32 x, s32 y, s32 z);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector2Int into a Vector3Int
		***********************************************************************************/
		Vector3Int(Vector2Int const& rhs);

		/*!*********************************************************************************
			\brief
			Desturctor
		***********************************************************************************/
		~Vector3Int(void) = default;

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector2Int into a Vector3Int
		***********************************************************************************/
		Vector3Int& operator=(Vector2Int const& rhs);

		/*********************************************************************************
											MATH ARITHMETIC
		*********************************************************************************/
		/*!*********************************************************************************
			\brief
			Add two 3D vectors together

			\param [in] rhs:
			3D vector on the right hand side to be added together with

			\return
			A reference to this vector after the summation of the two vectors
		***********************************************************************************/
		Vector3Int& operator+=(Vector3Int const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract two 3D vectors from each other

			\param [in] rhs:
			3D vector on the right hand side to be subtracted from

			\return
			A reference to this vector after subtracting the two vectors
		***********************************************************************************/
		Vector3Int& operator-=(Vector3Int const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar multiplication with 3D vector

			\param [in] rhs:
			Scalar value to be multiplied with

			\return
			A reference to this vector after multiplying the vector with the scalar
		***********************************************************************************/
		Vector3Int& operator*=(s32 rhs);

		/*!*********************************************************************************
			\brief
			Scalar division with 3D vector

			\param [in] rhs:
			Scalar value to be divided with

			\return
			A reference to this vector after dividing the vector by the scalar
		***********************************************************************************/
		Vector3Int& operator/=(s32 rhs);

		/*!*********************************************************************************
			\brief
			Negates the value of this vector

			\return
			A copy of this vector after negating the values
		***********************************************************************************/
		Vector3Int operator-(void) const;

		/*!*********************************************************************************
			\brief
			Dot product with another vector

			\param [in] rhs:
			Vector to be dotted with to produce the dot product

			\return
			The dot product between this vector and rhs
		***********************************************************************************/
		s32 Dot(Vector3Int const& rhs) const;

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

		/*!*********************************************************************************
			\brief
			Cross product with another vector

			\param [in] rhs:
			Vector to do cross product with

			\return
			A 3D vector after this vector cross product with rhs
		***********************************************************************************/
		Vector3Int Cross(Vector3Int const& rhs) const;

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
		bool operator==(Vector3Int const& rhs) const;

		/*!*********************************************************************************
			\brief
			Check if the two vectors are NOT the same

			\param [in] rhs:
			Vector on the right hand side to be checked with

			\return
			True if this vector is NOT the same as rhs, else false
		***********************************************************************************/
		bool operator!=(Vector3Int const& rhs) const;

		/*!*********************************************************************************
			\brief
			Set this vector's x, y and z component to zero
		***********************************************************************************/
		void SetZero(void);

		/*!*********************************************************************************
			\brief
			Check if this vector's x, y and z component are zero

			\return
			True if both x, y and z are zero, else false
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
		static s32 Distance(Vector3Int const& lhs, Vector3Int const& rhs);

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
		static s32 DistanceSq(Vector3Int const& lhs, Vector3Int const& rhs);

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
		static s32 Dot(Vector3Int const& lhs, Vector3Int const& rhs);

		/*!*********************************************************************************
			\brief
			Create a copy of a 3D vector with the maximum value from lhs and rhs

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			A copy of 3D vector with the maximum values from the two vectors
		***********************************************************************************/
		static Vector3Int Max(Vector3Int const& lhs, Vector3Int const& rhs);

		/*!*********************************************************************************
			\brief
			Create a copy of a 3D vector with the minimum value from lhs and rhs

			\param [in] lhs:
			First vector
			\param [in] rhs:
			Second vector

			\return
			A copy of 3D vector with the minimum values from the two vectors
		***********************************************************************************/
		static Vector3Int Min(Vector3Int const& lhs, Vector3Int const& rhs);

		/*!*********************************************************************************
			\brief
			Cross product between vector lhs and rhs

			\param [in] lhs:
			First vector to find the cross product with
			\param [in] rhs:
			Second vector to find the cross product with

			\return
			Cross product between lhs and rhs
		***********************************************************************************/
		static Vector3Int Cross(Vector3Int const& lhs, Vector3Int const& rhs);

		static Vector3Int const right;
		static Vector3Int const left;
		static Vector3Int const up;
		static Vector3Int const down;
		static Vector3Int const forward;
		static Vector3Int const back;
		static Vector3Int const one;
		static Vector3Int const zero;

	private:
		/*!*********************************************************************************
			\brief
			Swap the data of this vector with rhs

			\param [in, out] rhs:
			Vector to have it's data swapped with
		***********************************************************************************/
		void swap(Vector3Int& rhs);
	}; using Vec3Int = Vector3Int; using Vec3DInt = Vector3Int; using vec3Int = Vector3Int;

	/*!*********************************************************************************
		\brief
		Overloaded + operator to add two 3D vectors together

		\param [in] lhs:
		Vector on the left hand side of the + operator to be added with
		\param [in] rhs:
		Vector on the right hand side of the + operator to be added with

		\return
		3D vector containing the summation of lhs and rhs
	***********************************************************************************/
	Vector3Int operator+(Vector3Int const& lhs, Vector3Int const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded - operator to subtract two 3D vectors from each other

		\param [in] lhs:
		Vector on the left hand side of the - operator to be subtracted from
		\param [in] rhs:
		Vector on the right hand side of the - operator to be subtracted from

		\return
		3D vector containing the subtraction of lhs and rhs
	***********************************************************************************/
	Vector3Int operator-(Vector3Int const& lhs, Vector3Int const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded * operator to multiply a vector with a scalar

		\param [in] lhs:
		Vector on the left hand side of the * operator to be multiplied with
		\param [in] rhs:
		Scalar value on the right hand side of the * operator to be multiplied with

		\return
		3D vector containing the multiplication of vector lhs and scalar value rhs
	***********************************************************************************/
	Vector3Int operator*(Vector3Int const& lhs, s32 rhs);

	/*!*********************************************************************************
		\brief
		Overloaded * operator to multiply a vector with a scalar

		\param [in] lhs:
		Scalar value on the left hand side of the * operator to be multiplied with
		\param [in] rhs:
		Vector on the right hand side of the * operator to be multiplied with

		\return
		3D vector containing the multiplcaition of scalar value lhs and vector rhs
	***********************************************************************************/
	Vector3Int operator*(s32 lhs, Vector3Int const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded / operator to divide a vector by scalar

		\param [in] lhs:
		Vector on the right hand side of the / operator to be divided with
		\param [in] rhs:
		Scalar value on the right hand side of the / operator to divide the vector by

		\return
		3D vector containing the division of vector lhs by scalar value rhs
	***********************************************************************************/
	Vector3Int operator/(Vector3Int const& lhs, s32 rhs);

	/*!*********************************************************************************
		\brief
		To output the data of Vector3 to the output stream

		\param [in] os:
		Reference to the output stream to stream the Vector3 data to
		\param [in] rhs:
		Vector data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vector3Int const& rhs);

}

#endif //!VECTOR3INT_H
