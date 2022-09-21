/*!
file:	Matrix4x4.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain a 4x4 Matrix that is inherited from a base matrix class. The file
		also provides function declaration to constructing a Translation, Scale and Rotation matrix
		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	MATRIX_4X4_H
#define MATRIX_4X4_H

namespace ALEngine::Math
{
	class Matrix4x4
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
		Matrix4x4(value_type value);

		/*!*********************************************************************************
			\brief
			Constructor to be initialised with Vector4
			\param [in] r1:
			First row of the matrix
			\param [in] r2:
			Second row of the matrix
			\param [in] r3:
			Third row of the matrix
			\param [in] r4:
			Fourth row of the matrix
		***********************************************************************************/
		Matrix4x4(Vector4 r1 = Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4 r2 = Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4 r3 = Vector4(0.0f, 0.0f, 1.0f, 0.0f), Vector4 r4 = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

		/*!*********************************************************************************
			\brief
			Destructor
		***********************************************************************************/
		~Matrix4x4(void) = default;

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
			\brief
			Add the two 3x3 matrix together
			\param [in] rhs:
			Matrix on the right hand side to be added together with
			\return
			A reference to this class's matrix after the summation of the two matrix
		***********************************************************************************/
		Matrix4x4& operator+=(Matrix4x4 const& rhs);

		/*!*********************************************************************************
			\brief
			Subtract the two 3x3 matrix from each other
			\param [in] rhs:
			Matrix on the right hand side to be subtracted from
			\return
			A reference to this class's matrix after subtracting the two matrix
		***********************************************************************************/
		Matrix4x4& operator-=(Matrix4x4 const& rhs);

		/*!*********************************************************************************
			\brief
			Row major matrix multiplication of the 3x3 matrix
			\param [in] rhs:
			Matrix on the right hand side to be multiplied with
			\return
			A reference to this class's matrix after doing matrix multiplication
		***********************************************************************************/
		Matrix4x4& operator*=(Matrix4x4 const& rhs);

		/*!*********************************************************************************
			\brief
			Scalar matrix multiplication
			\param [in] rhs:
			Scalar value to be multiplied with
			\return
			A reference to this class's matrix after doing scalar multiplication
		***********************************************************************************/
		Matrix4x4& operator*=(value_type rhs);

		/*!*********************************************************************************
			\brief
			Returns a const pointer to the data for matrix
		***********************************************************************************/
		const_pointer value_ptr(void) const;

		/*!*********************************************************************************
			\brief
			Turn a 4x4 matrix into a translation matrix
			\param [in] x:
			Position along the x-axis
			\param [in] y:
			Position along the y-axis
			\param [in] z:
			Position along the z-axis
			\return
			A transposed Translation matrix (for OpenGL use)
		***********************************************************************************/
		static Matrix4x4 Translate(f32 x, f32 y, f32 z);

		/*!*********************************************************************************
			\brief
			Turn a 4x4 into a translation matrix
			\param [in] rhs:
			Vector3 containing the x, y and z position
			\return
			A transposed Translation matrix (for OpenGL use)
		***********************************************************************************/
		static Matrix4x4 Translate(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Turn a 4x4 matrix into a scale matrix
			\param [in] x:
			Scale factor along the x-axis
			\param [in] y:
			Scale factor along the y-axis
			\return
			A Scale matrix
		***********************************************************************************/
		static Matrix4x4 Scale(f32 x, f32 y, f32 z);

		/*!*********************************************************************************
			\brief
			Turn a 4x4 matrix into a scale matrix
			\param [in] rhs:
			Vector3 containing the x and y scalar factor
			\return
			A Scale matrix
		***********************************************************************************/
		static Matrix4x4 Scale(Vector3 const& rhs);

		/*!*********************************************************************************
			\brief
			Turn a 4x4 matrix into a rotation matrix
			\param [in] deg:
			Angle in degree
			\param [in] axis:
			The axis to be rotated about
			\return
			A rotation matrix
		***********************************************************************************/
		static Matrix4x4 Rotation(f32 angle, Vector3 const& axis);

		/*!*********************************************************************************
			\brief
			Construct a Orthographics projection matrix
			\param [in] left:
			Minimum x value of the projection matrix
			\param [in] right:
			Maximum x value of the projection matrix
			\param [in] bottom:
			Minimum y value of the projection matrix
			\param [in] top:
			Minimum y value of the projection matrix
			\return
			A Orthographic projection matrix
		***********************************************************************************/
		static Matrix4x4 Ortho(f32 left, f32 right, f32 bottom, f32 top);

		/*!*********************************************************************************
			\brief
			Construct a Orthographics projection matrix
			\param [in] left:
			Minimum x value of the projection matrix
			\param [in] right:
			Maximum x value of the projection matrix
			\param [in] bottom:
			Minimum y value of the projection matrix
			\param [in] top:
			Minimum y value of the projection matrix
			\param [in] zNear:
			Distance of how near the camera can see
			\param [in] zFar:
			Distance of how far the camera can see
			\return
			A Orthographic projection matrix
		***********************************************************************************/
		static Matrix4x4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);

		/*!*********************************************************************************
			\brief
			Construct a Perspective projection matrix
			\param [in] fov:
			Field of view of the perspective projection matrix
			\param [in] aspect:
			Aspect ratio of the view matrix -> width / height
			\param [in] zNear:
			Distance of how near the camera can see
			\param [in] zFar:
			Distance of how far the camera can see
			\return
			A Perspective projection matrix
		***********************************************************************************/
		static Matrix4x4 Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar);

		/*!*********************************************************************************
			\brief
			Construct a view matrix
			\param [in] eye:
			Position of where the camera is
			\param [in] center:
			Center of where the camera is looking at
			\param [in] up:
			Up vector of the camera
		***********************************************************************************/
		static Matrix4x4 LookAt(Vector3 pos, Vector3 tgt, Vector3 up);

		static Matrix4x4 Model(Vector3 const& pos, Vector3 const& scale, f32 rot);

	private:
		Vector4 mat[4];
	}; using Mat4 = Matrix4x4; using Mtx4 = Matrix4x4; using Matrix4 = Matrix4x4; using mat4 = Matrix4x4;

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
	Matrix4x4 operator+(Matrix4x4 const& lhs, Matrix4x4 const& rhs);

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
	Matrix4x4 operator-(Matrix4x4 const& lhs, Matrix4x4 const& rhs);

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
	Matrix4x4 operator*(Matrix4x4 const& lhs, Matrix4x4 const& rhs);

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
	Matrix4x4 operator*(Matrix4x4 const& lhs, typename Matrix4x4::value_type rhs);

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
	Matrix4x4 operator*(typename Matrix4x4::value_type lhs, Matrix4x4 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with Vector3
		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Vector3 on the left hand side of the * operator to multiply with
	***********************************************************************************/
	Vector3 operator*(Matrix4x4 const& lhs, Vector3 const& rhs);

	/*!*********************************************************************************
		\brief
		Matrix multiplication with Vector4
		\param [in] lhs:
		Matrix on the left hand side of the * operator to multiply with
		\param [in] rhs:
		Vector4 on the left hand side of the * operator to multiply with
	***********************************************************************************/
	Vector4 operator*(Matrix4x4 const& lhs, Vector4 const& rhs);

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