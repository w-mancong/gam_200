#ifndef MATRIX_2X2_H
#define MATRIX_2X2_H

namespace ALEngine::Math
{
	class Matrix2x2
	{
	public:
		using value_type = f32;
		using reference = value_type&;
		using const_reference = value_type const&;
		using pointer = value_type*;
		using const_pointer = value_type const*;
		using size_type = u64;
		/*!*********************************************************************************
			\brief
			Constructor that takes in a single value_type variable that initializes the
			class to have value_type diagonally
			\param [in]
			Value to be placed diagonally
		***********************************************************************************/
		Matrix2x2(value_type value);

		/*!*********************************************************************************
			\brief
			Constructor to be initialised with Vector4
			\param [in] r1:
			First row of the matrix
			\param [in] r2:
			Second row of the matrix
		***********************************************************************************/
		Matrix2x2(Vector2 r1 = Vector2(1.0f, 0.0f), Vector2 r2 = Vector2(0.0f, 1.0f));

		/*!*********************************************************************************
			\brief
			Destructor
		***********************************************************************************/
		~Matrix2x2(void) = default;

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
		vec2& operator()(size_type row);

		/*!*********************************************************************************
			\brief return a const reference to matrix's row

			\param [in] row: Row of the matrix to retrieve the data from
		***********************************************************************************/
		vec2 const& operator()(size_type row) const;

		/*!*********************************************************************************
			\brief
			Add the two 3x3 matrix together
			\param [in] rhs:
			Matrix on the right hand side to be added together with
			\return
			A reference to this class's matrix after the summation of the two matrix
		***********************************************************************************/
		Matrix2x2& operator+=(Matrix2x2 const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract the two 3x3 matrix from each other
			\param [in] rhs:
			Matrix on the right hand side to be subtracted from
			\return
			A reference to this class's matrix after subtracting the two matrix
		***********************************************************************************/
		Matrix2x2& operator-=(Matrix2x2 const& rhs);

		/*!*********************************************************************************
			\brief
			Row major matrix multiplication of the 3x3 matrix
			\param [in] rhs:
			Matrix on the right hand side to be multiplied with
			\return
			A reference to this class's matrix after doing matrix multiplication
		***********************************************************************************/
		Matrix2x2& operator*=(Matrix2x2 const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar matrix multiplication
			\param [in] rhs:
			Scalar value to be multiplied with
			\return
			A reference to this class's matrix after doing scalar multiplication
		***********************************************************************************/
		Matrix2x2& operator*=(value_type rhs);

		/*!*********************************************************************************
			\brief Construct and return an inverse matrix

			\param [in] matrix: Matrix to be inversed
		***********************************************************************************/
		Matrix2x2 Inverse(void) const;

		/*!*********************************************************************************
			\brief Construct and return a column major inverse matrix

			\param [in] matrix: Matrix to be inversed
		***********************************************************************************/
		Matrix2x2 InverseT(void) const;

		/*!*********************************************************************************
			\brief Construct and return a transpose matrix

			\param [in] matrix: Matrix to be transposed
		***********************************************************************************/
		Matrix2x2 Transpose(void) const;

		/*!*********************************************************************************
			\brief Calculate and return the determinant of mat

			\param [in] matrix: To calculate the determinant of the matrix
		***********************************************************************************/
		f32 Determinant(void) const;

		/*!*********************************************************************************
			\brief Construct and return an inverse matrix

			\param [in] matrix: Matrix to be inversed
		***********************************************************************************/
		static Matrix2x2 Inverse(Matrix2x2 const& mat);

		/*!*********************************************************************************
			\brief Construct and return a column major inverse matrix

			\param [in] matrix: Matrix to be inversed
		***********************************************************************************/
		static Matrix2x2 InverseT(Matrix2x2 const& mat);

		/*!*********************************************************************************
			\brief Construct and return a transpose matrix

			\param [in] matrix: Matrix to be transposed
		***********************************************************************************/
		static Matrix2x2 Transpose(Matrix2x2 const& mat);

		/*!*********************************************************************************
			\brief Calculate and return the determinant of mat

			\param [in] matrix: To calculate the determinant of the matrix
		***********************************************************************************/
		static f32 Determinant(Matrix2x2 const& mat);

		Vector2 mat[2];
	}; using Mat2 = Matrix2x2; using Mtx2 = Matrix2x2; using Matrix2 = Matrix2x2; using mat2 = Matrix2x2;

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
	Matrix2x2 operator+(Matrix2x2 const& lhs, Matrix2x2 const& rhs);

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
	Matrix2x2 operator-(Matrix2x2 const& lhs, Matrix2x2 const& rhs);

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
	Matrix2x2 operator*(Matrix2x2 const& lhs, Matrix2x2 const& rhs);

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
	Matrix2x2 operator*(Matrix2x2 const& lhs, typename Matrix2x2::value_type rhs);

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
	Matrix2x2 operator*(typename Matrix2x2::value_type lhs, Matrix2x2 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with Vector4
		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Vector4 on the left hand side of the * operator to multiply with
	***********************************************************************************/
	Vector2 operator*(Matrix2x2 const& lhs, Vector2 const& rhs);

	/*!*********************************************************************************
		\brief
		To output data of matrix to the output stream
		\param [in] os:
		Reference to the output stream to stream the matrix data to
		\param [in] rhs:
		Matrix data to be streamed to the output stream os
	***********************************************************************************/
	std::ostream& operator<<(std::ostream& os, Matrix4x4 const& rhs);
}

#endif