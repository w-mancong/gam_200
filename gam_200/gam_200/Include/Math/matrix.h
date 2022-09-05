/*!
file:	matrix.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain function declarations & definition for a templated matrix class

        All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
namespace ALEngine
{
    namespace Exceptions
    {
        class InvalidDimension : public std::exception
        {
        public:
            /*!*********************************************************************************
                \brief
                Constructor for InvalidDimension

                \param [in] row:
                The row of the invalid matrix
                \param [in] cols:
                The column of the invalud matrix
                \param [in] s:
                Special message for special cases
            ***********************************************************************************/
            InvalidDimension(s64 rows, s64 cols, const char* s = nullptr);

            /*!*********************************************************************************
                \brief
                To print out the error message when the exception is caught
            ***********************************************************************************/
            virtual const char* what(void) const noexcept;

        private:
            std::string msg;
        };

        class IndexOutOfBounds : public std::exception
        {
        public:
            /*!*********************************************************************************
                \brief
                Constructor for IndexOutOfBounds

                \param [in] row:
                Row index that is being accessed
                \param [in] R:
                Row dimension of the matrix
                \param [in] col:
                Column index that is being accessed
                \param [in] C:
                Column dimension of the matrix
            ***********************************************************************************/
            IndexOutOfBounds(s64 row, s64 R, s64 col, s64 C);

            /*!*********************************************************************************
                \brief
                To print out the error message when the exception is caught
            ***********************************************************************************/
            virtual const char* what(void) const noexcept;

        private:
            std::string msg;
        };

        class IncompatibleMatrices : public std::exception
        {
        public:
            /*!*********************************************************************************
                \brief
                Constructor for IncompatibleMatrices

                \param [in] operation:
                The type of mathematic arithmetic that is is trying to perform
                \param pin] l_rows:
                Row of matrix on the left hand side
                \param [in] l_cols:
                Column of the matrix on the left hand side
                \param [in] r_rows:
                Row of matrix on the right hand side
                \param [in] r_cols:
                Column of matrix on the right hand side
            ***********************************************************************************/
            IncompatibleMatrices(const char* operation, s64 l_rows, s64 l_cols, s64 r_rows, s64 r_cols);

            /*!*********************************************************************************
                \brief
                To print out the error message when the exception is caught
            ***********************************************************************************/
            virtual const char* what(void) const noexcept;

        private:
            std::string msg;
        };
    }

	namespace Math
	{
        class Vector2; class Vector3;
        class matrix
        {
        public:
            using value_type        = f32;
            using reference         = value_type&;
            using const_reference   = value_type const&;
            using pointer           = value_type*;
            using const_pointer     = value_type const*;
            using size_type         = s64;
        public:
            /*!*********************************************************************************
                \brief
                Constructor that takes in the Row and Column for constructing a matrix

                \param [in] R:
                Row of this matrix class
                \param [in] C:
                Column of this matrix class
            ***********************************************************************************/
            matrix(size_type R, size_type C);

            /*!*********************************************************************************
                \brief
                Conversion constructor that converts Vector2 to a 3x1 matrix
            ***********************************************************************************/
            matrix(Vector2 const& rhs);

            /*!*********************************************************************************
                \brief
                Conversion constructor that converts Vector3 to a 4x1 matrix
            ***********************************************************************************/
            matrix(Vector3 const& rhs);

            /*!*********************************************************************************
                \brief
                Copy constructor
            ***********************************************************************************/
            matrix(matrix const& rhs);

            /*!*********************************************************************************
                \brief
                Move constructor
            ***********************************************************************************/
            matrix(matrix&& rhs) noexcept;

            /*!*********************************************************************************
                \brief
                Destructor
            ***********************************************************************************/
            virtual ~matrix(void);

            /*!*********************************************************************************
                \brief
                Copy assignment operator
            ***********************************************************************************/
            matrix& operator=(matrix const& rhs);

            /*!*********************************************************************************
                \brief
                Move assignment operator
            ***********************************************************************************/
            matrix& operator=(matrix&& rhs) noexcept;

            /*!*********************************************************************************
                \brief
                Overloaded assignment operator to convert Vector2 to a 3x1 matrix
            ***********************************************************************************/
            matrix& operator=(Vector2 const& rhs);

            /*!*********************************************************************************
                \brief
                Overloaded assignment operator to convert Vector3 to a 4x1 matrix
            ***********************************************************************************/
            matrix& operator=(Vector3 const& rhs);

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
                Checks if both the matrix are equal to each other

                \param [in] rhs:
                Matrix on the right hand side to be compared with

                \return
                True if this matrix and rhs are the same, else false
            ***********************************************************************************/
            bool operator==(matrix const& rhs) const;

            /*!*********************************************************************************
                \brief
                Checks if both the matrix are NOT equal to each other

                \param [in] rhs:
                Matrix on the right hand side to be compared with

                \return
                True if this matrix and rhs are NOT the same, else false
            ***********************************************************************************/
            bool operator!=(matrix const& rhs) const;

            /*!*********************************************************************************
                \brief
                Add the two matrix together

                \param [in] rhs:
                Matrix on the right hand side to be added together with

                \return
                A reference to this class's matrix after the summation of the two matrix
            ***********************************************************************************/
            matrix& operator+=(matrix const& rhs);

            /*!*********************************************************************************
                \brief
                Subtract the two matrix from each other

                \param [in] rhs:
                Matrix on the right hand side to be subtracted from

                \return
                A reference to this class's matrix after subtracting the two matrix
            ***********************************************************************************/
            matrix& operator-=(matrix const& rhs);

            /*!*********************************************************************************
                \brief
                Row major matrix multiplication

                \param [in] rhs:
                Matrix on the right hand side to be multiplied with

                \return
                A reference to this class's matrix after doing matrix multiplication
            ***********************************************************************************/
            matrix& operator*=(matrix const& rhs);

            /*!*********************************************************************************
                \brief
                Scalar matrix multiplication

                \param [in] rhs:
                Scalar value to be multiplied with

                \return
                A reference to this class's matrix after doing scalar multiplication
            ***********************************************************************************/
            matrix& operator*=(value_type rhs);

            /*!*********************************************************************************
                \brief
                Return the number of rows for this matrix
            ***********************************************************************************/
            size_type Rows(void) const;

            /*!*********************************************************************************
                \brief
                Return the number of columns for this matrix
            ***********************************************************************************/
            size_type Cols(void) const;

            /*!*********************************************************************************
                \brief
                Transpose the matrix

                \return
                A reference to this class's matrix after transposing the matrix
            ***********************************************************************************/
            matrix& Transpose(void);

            /*!*********************************************************************************
                \brief
                Inverse this matrix

                \return 
                A reference to this class after inversing the matrix
            ***********************************************************************************/
            matrix& Inverse(void);

            /*!*********************************************************************************
                \brief
                Making this class an identity matrix if it is a N x N matrix
            ***********************************************************************************/
            void Identity(void);

            /*!*********************************************************************************
                \brief
                Return the determinant of this matrix
            ***********************************************************************************/
            value_type Determinant(void) const;

            /*!*********************************************************************************
                \brief
                Returns a const pointer to the data for matrix
            ***********************************************************************************/
            const_pointer value_ptr(void) const;

        private:
            /*!*********************************************************************************
                \brief
                Helper function to calculates the determinant of this matrix

                \param [in] m:
                Bar matrix of the determinant
                \param [in] n:
                Current row and column of matrix m

                \return
                The determinant for matrix m
            ***********************************************************************************/
            value_type Determinant(matrix const& m, size_type n) const;

            /*!*********************************************************************************
                \brief
                Get the bar matrix from the src matrix

                \param [out] dst:
                Store the bar matrix of src into dst
                \param [in] src:
                Source of the matrix to construct the bar matrix from
                \param [in] row:
                Current row to be ignored
                \param [in] col:
                Current column to be ignored
            ***********************************************************************************/
            void BarMatrix(matrix& dst, matrix const& src, size_type row, size_type col) const;

            /*!*********************************************************************************
                \brief
                Return the value at indexed at row and col

                \param [in] row:
                Row index
                \param [in] col:
                Column index

                /return
                A const reference to the value indexed at row and col
            ***********************************************************************************/
            const_reference cget(size_type row, size_type col) const;

            /*!*********************************************************************************
                \brief
                Swap the two matrix around

                \param [in, out] rhs:
                The matrix to have it's data swapped with this class
            ***********************************************************************************/
            void swap(matrix& rhs);

            value_type* mtx;
            size_type R, C;
        };

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
        matrix operator+(matrix const& lhs, matrix const& rhs);

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
        matrix operator-(matrix const& lhs, matrix const& rhs);

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
        matrix operator*(matrix const& lhs, matrix const& rhs);

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
        matrix operator*(matrix const& lhs, typename matrix::value_type rhs);

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
        matrix operator*(typename matrix::value_type lhs, matrix const& rhs);

        /*!*********************************************************************************
            \brief
            To output data of matrix to the output stream

            \param [in] os:
            Reference to the output stream to stream the matrix data to
            \param [in] rhs:
            Matrix data to be streamed to the output stream os
        ***********************************************************************************/
        std::ostream& operator<<(std::ostream& os, matrix const& rhs);
	}
}
#endif