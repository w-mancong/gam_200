/*!
file:	Matrix3x3.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain a 3x3 Matrix that is inherited from a base matrix class. The file
		also provides function declaration to constructing a Translation, Scale and Rotation matrix

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	MATRIX_3X3_H
#define MATRIX_3X3_H

#include "matrix.h"
namespace ALEngine::Math
{
	class Matrix3x3
	{
	public:
		using value_type = f32;
		using reference = value_type&;
		using const_reference = value_type const&;
		using pointer = value_type*;
		using const_pointer = value_type const*;
		using size_type = s64;
		/*!*********************************************************************************
			\brief
			Constructor that takes in a single value_type variable that initializes the
			class to have value_type diagonally

			\param [in]
			Value to be placed diagonally
		***********************************************************************************/
		Matrix3x3(value_type value);

		/*!*********************************************************************************
			\brief
			Constructor to be initialised with Vector3

			\param [in] r1:
			First row of the matrix
			\param [in] r2:
			Second row of the matrix
			\param [in] r3:
			Third row of the matrix
		***********************************************************************************/
		Matrix3x3(Vector3 r1 = Vector3(1.0f, 0.0f, 0.0f), Vector3 r2 = Vector3(0.0f, 1.0f, 0.0f), Vector3 r3 = Vector3(0.0f, 0.0f, 1.0f));

		/*!*********************************************************************************
			\brief
			Destructor
		***********************************************************************************/
		~Matrix3x3(void) = default;

		/*!*********************************************************************************
			\brief
			Add the two 3x3 matrix together

			\param [in] rhs:
			Matrix on the right hand side to be added together with

			\return
			A reference to this class's matrix after the summation of the two matrix
		***********************************************************************************/
		Matrix3x3& operator+=(Matrix3x3 const& rhs);

		/*!*********************************************************************************
			\brief
            Subtract the two 3x3 matrix from each other

            \param [in] rhs:
            Matrix on the right hand side to be subtracted from

            \return
            A reference to this class's matrix after subtracting the two matrix
		***********************************************************************************/
		Matrix3x3& operator-=(Matrix3x3 const& rhs);

		/*!*********************************************************************************
			\brief
			Row major matrix multiplication of the 3x3 matrix

			\param [in] rhs:
			Matrix on the right hand side to be multiplied with

			\return
			A reference to this class's matrix after doing matrix multiplication
		***********************************************************************************/
		Matrix3x3& operator*=(Matrix3x3 const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar matrix multiplication

			\param [in] rhs:
			Scalar value to be multiplied with

			\return
			A reference to this class's matrix after doing scalar multiplication
		***********************************************************************************/
		Matrix3x3& operator*=(value_type rhs);

		/*!*********************************************************************************
			\brief
			Overloaded () operator to access and modify the matrix

			\param [in] row:
			Row of the matrix to retrieve data from
			\param [in] col:
			Column of the matrix to retrieve data from
		***********************************************************************************/
		reference operator()(size_type row, size_type col);

		/*!*********************************************************************************
			\brief
			Overloaded () operator to access the matrix

			\param [in] row:
			Row of the matrix to retrieve the data from
			\param [in] col:
			Column of the matrix to retrieve the data from
		***********************************************************************************/
		const_reference operator()(size_type row, size_type col) const;

		/*!*********************************************************************************
			\brief return a const reference to matrix's row

			\param [in] row: Row of the matrix to retrieve the data from
		***********************************************************************************/
		vec3& operator()(size_type row);

		/*!*********************************************************************************
			\brief return a const reference to matrix's row

			\param [in] row: Row of the matrix to retrieve the data from
		***********************************************************************************/
		vec3 const& operator()(size_type row) const;

		/*!*********************************************************************************
			\brief
			Turn a 3x3 matrix into a translation matrix

			\param [in] x:
			Position along the x-axis
			\param [in] y:
			Position along the y-axis

			\return
			A transposed Translation matrix (for OpenGL use)
		***********************************************************************************/
		static Matrix3x3 Translate(f32 x, f32 y);

		/*!*********************************************************************************
			\brief
			Turn a 3x3 into a translation matrix

			\param [in] rhs:
			Vector2 containing the x and y position

			\return
			A transposed Translation matrix (for OpenGL use)
		***********************************************************************************/
		static Matrix3x3 Translate(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Turn a 3x3 matrix into a scale matrix

			\param [in] x:
			Scale factor along the x-axis
			\param [in] y:
			Scale factor along the y-axis

			\return 
			A Scale matrix
		***********************************************************************************/
		static Matrix3x3 Scale(f32 x, f32 y);

		/*!*********************************************************************************
			\brief
			Turn a 3x3 matrix into a scale matrix
				
			\param [in] rhs:
			Vector2 containing the x and y scalar factor

			\return
			A Scale matrix
		***********************************************************************************/
		static Matrix3x3 Scale(Vector2 const& rhs);

		/*!*********************************************************************************
			\brief
			Turn a 3x3 matrix into a rotation matrix

			\param [in] deg:
			Angle in degree

			\return
			A rotation matrix
		***********************************************************************************/
		static Matrix3x3 Rotation(f32 deg);

		/*!*********************************************************************************
			\brief Construct and return an inverse matrix

			\param [in] matrix: Matrix to be inversed
		***********************************************************************************/
		static Matrix3x3 Inverse(Matrix3x3 const& mat);

	private:
		Vector3 mat[3];
	}; using Mat3 = Matrix3x3; using Mtx3 = Matrix3x3; using mat3 = Matrix3x3;

	/*!*********************************************************************************
		\brief
		Overloaded + operator to add two matrix together

		\param [in] lhs:
		Matrix on the left hand side of the + operator to be added with
		\param [in] rhs:
		Matrix on the right hand side of the + operator to be added with

		\return
		Matrix containing the summation of lhs and rhs
	***********************************************************************************/
	Matrix3x3 operator+(Matrix3x3 const& lhs, Matrix3x3 const& rhs);

	/*!*********************************************************************************
		\brief
		Overloaded - operator to subtract two matrix from each other

		\param [in] lhs:
		Matrix on the left hand side of the - operator to be subtracted from
		\param [in] rhs:
		Matrix on the right hand side of the - operator to be subtracted from

		\return
		Matrix containing the subtraction of lhs from rhs
	***********************************************************************************/
	Matrix3x3 operator-(Matrix3x3 const& lhs, Matrix3x3 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication between lhs and rhs

		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Matrix on the right hand side of the * operator to multiply with

		\return
		Matrix containing the multiplication of matrix lhs and rhs
	***********************************************************************************/
	Matrix3x3 operator*(Matrix3x3 const& lhs, Matrix3x3 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with a scalar

		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Scalar value on the right hand side of the * operator to multiply with

		\return
		Matrix containing the multiplication of matrix lhs and scalar value rhs
	***********************************************************************************/
	Matrix3x3 operator*(Matrix3x3 const& lhs, typename Matrix3x3::value_type rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with a scalar

		\param [in] lhs:
		Scalar value on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Matrix on the right hand side of the * operator to multiply with

		\return
		Matrix containing the multiplication of scalar value lhs and matrix rhs
	***********************************************************************************/
	Matrix3x3 operator*(typename Matrix3x3::value_type lhs, Matrix3x3 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with Vector2

		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Vector2 on the left hand side of the * operator to multiply with
	***********************************************************************************/
	Vector2 operator*(Matrix3x3 const& lhs, Vector2 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with Vector3

		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Vector2 on the left hand side of the * operator to multiply with
	***********************************************************************************/
	Vector3 operator*(Matrix3x3 const& lhs, Vector3 const& rhs);

	/*!*********************************************************************************
		\brief
		To output data of matrix to the output stream
		\param [in] os:
		Reference to the output stream to stream the matrix data to
		\param [in] rhs:
		Matrix data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Matrix3x3 const& rhs);
}

#endif