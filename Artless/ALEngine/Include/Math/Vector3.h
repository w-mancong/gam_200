/*!
file:	Vector3.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain class definition for representing a 3D vector

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	VECTOR3_H
#define VECTOR3_H

namespace ALEngine::Math
{
	class Vector2; class Vector4;
	class Vector3
	{
	public:
		f32 x, y, z;
	public:
		/*!*********************************************************************************
			\brief
			Constructor
		***********************************************************************************/
		Vector3(void);

		/*!*********************************************************************************
			\brief
			Constructor that takes in two floats for initalization

			\param [in] x:
			X component of the vector
			\param [in] y:
			Y component of the vector
			\param [in] z:
			Z component of the vector
		***********************************************************************************/
		Vector3(f32 x, f32 y, f32 z);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector2 into a Vector3
		***********************************************************************************/
		Vector3(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Conversion constructor that converts a Vector4 into a Vector3
		***********************************************************************************/
		Vector3(Vector4 const& rhs);

		/*!*********************************************************************************
			\brief
			Desturctor
		***********************************************************************************/
		~Vector3(void) = default;

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector2 into a Vector3
		***********************************************************************************/
		Vector3& operator=(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Overloaded assignment operator that converts a Vector4 into a Vector3
		***********************************************************************************/
		Vector3& operator=(Vector4 const& rhs);

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
		Vector3& operator+=(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract two 3D vectors from each other

			\param [in] rhs:
			3D vector on the right hand side to be subtracted from

			\return
			A reference to this vector after subtracting the two vectors
		***********************************************************************************/
		Vector3& operator-=(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar multiplication with 3D vector

			\param [in] rhs:
			Scalar value to be multiplied with

			\return
			A reference to this vector after multiplying the vector with the scalar
		***********************************************************************************/
		Vector3& operator*=(f32 rhs);

		/*!*********************************************************************************
			\brief
			Scalar division with 3D vector

			\param [in] rhs:
			Scalar value to be divided with

			\return
			A reference to this vector after dividing the vector by the scalar
		***********************************************************************************/
		Vector3& operator/=(f32 rhs);

		/*!*********************************************************************************
			\brief
			Negates the value of this vector

			\return
			A copy of this vector after negating the values
		***********************************************************************************/
		Vector3 operator-(void) const;

		/*!*********************************************************************************
			\brief
			Dot product with another vector

			\param [in] rhs:
			Vector to be dotted with to produce the dot product

			\return
			The dot product between this vector and rhs
		***********************************************************************************/
		f32 Dot(Vector3 const& rhs) const;

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
		Vector3 Normalize(void) const;

		/*!*********************************************************************************
			\brief
			Cross product with another vector

			\param [in] rhs:
			Vector to do cross product with

			\return
			A 3D vector after this vector cross product with rhs
		***********************************************************************************/
		Vector3 Cross(Vector3 const& rhs) const;

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
		bool operator==(Vector3 const& rhs) const;

		/*!*********************************************************************************
			\brief
			Check if the two vectors are NOT the same

			\param [in] rhs:
			Vector on the right hand side to be checked with

			\return
			True if this vector is NOT the same as rhs, else false
		***********************************************************************************/
		bool operator!=(Vector3 const& rhs) const;

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
			Find the angle between two vectors

			\param [in] from:
			Angle from this vector (1st vector)
			\param [in] to:
			To the angle of this vector (2nd vector)

			\return
			Angle in degrees between the two vectors
		***********************************************************************************/
		static f32 Angle(Vector3 const& from, Vector3 const& to);

		/*!*********************************************************************************
			\brief
			Clamp the magnitude of lhs to the maxLength

			\param [in] lhs:
			To return a copy of this vector with it's magnitude clamp with maxLength

			\return
			A copy of lhs with magnitude maxLength
		***********************************************************************************/
		static Vector3 ClampMagnitude(Vector3 const& lhs, f32 maxLength);

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
		static f32 Distance(Vector3 const& lhs, Vector3 const& rhs);

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
		static f32 DistanceSq(Vector3 const& lhs, Vector3 const& rhs);

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
		static f32 Dot(Vector3 const& lhs, Vector3 const& rhs);

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
		static Vector3 Max(Vector3 const& lhs, Vector3 const& rhs);

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
		static Vector3 Min(Vector3 const& lhs, Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Normalize the vector rhs

			\param [in] rhs:
			The vector to be normalize

			\return
			A copy of rhs in it's normalize form
		***********************************************************************************/
		static Vector3 Normalize(Vector3 const& rhs);

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
		static Vector3 Cross(Vector3 const& lhs, Vector3 const& rhs);

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
	private:
		/*!*********************************************************************************
			\brief
			Swap the data of this vector with rhs

			\param [in, out] rhs:
			Vector to have it's data swapped with
		***********************************************************************************/
		void swap(Vector3& rhs);
	}; using Vec3 = Vector3; using Vec3D = Vector3; using vec3 = Vector3;

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
	Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs);

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
	Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs);

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
	Vector3 operator*(Vector3 const& lhs, f32 rhs);

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
	Vector3 operator*(f32 lhs, Vector3 const& rhs);

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
	Vector3 operator/(Vector3 const& lhs, f32 rhs);

	/*!*********************************************************************************
		\brief
		To output the data of Vector3 to the output stream

		\param [in] os:
		Reference to the output stream to stream the Vector3 data to
		\param [in] rhs:
		Vector data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vector3 const& rhs);
}

#endif