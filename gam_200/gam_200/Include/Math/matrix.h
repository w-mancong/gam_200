/*!*****************************************************************************
\file matrix.hpp
\author Wong Man Cong
\par DP email: w.mancong\@digipen.edu
\date 16-03-2022
\brief
This file contain function declarations & definition for a templated matrix
class
*******************************************************************************/
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
namespace ManCong
{
    namespace Exception
    {
        class InvalidDimension : public std::exception
        {
        public:
            InvalidDimension(long long rows, long long cols, const char* s = nullptr);
            ~InvalidDimension(void) = default;

            virtual const char* what(void) const noexcept;

        private:
            std::string msg;
        };

        class IndexOutOfBounds : public std::exception
        {
        public:
            IndexOutOfBounds(long long row, long long R, long long col, long long C);
            ~IndexOutOfBounds(void) = default;

            virtual const char* what(void) const noexcept;

        private:
            std::string msg;
        };

        class IncompatibleMatrices : public std::exception
        {
        public:
            IncompatibleMatrices(const char* operation, long long l_rows, long long l_cols, long long r_rows, long long r_cols);
            ~IncompatibleMatrices(void) = default;

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
            using value_type = float;
            using reference = value_type&;
            using const_reference = value_type const&;
            using size_type = long long;
        public:
            matrix(size_type R, size_type C);
            matrix(Vector2 const& rhs);
            matrix(Vector3 const& rhs);
            virtual ~matrix(void);
            matrix(matrix const& rhs);
            matrix(matrix&& rhs) noexcept;
            matrix& operator=(matrix const& rhs);
            matrix& operator=(matrix&& rhs) noexcept;
            matrix& operator=(Vector2 const& rhs);
            matrix& operator=(Vector3 const& rhs);

            reference       operator()(size_type row, size_type col);
            const_reference operator()(size_type row, size_type col) const;

            bool            operator==(matrix const& rhs) const;
            bool            operator!=(matrix const& rhs) const;

            matrix&         operator+=(matrix const& rhs);

            matrix&         operator-=(matrix const& rhs);

            matrix&         operator*=(matrix const& rhs);
            matrix&         operator*=(value_type rhs);

            size_type       Rows(void) const;
            size_type       Cols(void) const;

            matrix&         Transpose(void);
            matrix&         Inverse(void);
            void            Indentity(void);
            value_type      Determinant(void) const;

        private:
            value_type      Determinant(matrix const& m, size_type n) const;
            void            BarMatrix(matrix& dst, matrix const& src, size_type row, size_type col) const;
            const_reference cget(size_type row, size_type col) const;
            void            swap(matrix& rhs);

            value_type* mtx;
            size_type R, C;
        };

        matrix operator+(matrix const& lhs, matrix const& rhs);
        matrix operator-(matrix const& lhs, matrix const& rhs);
        matrix operator*(matrix const& lhs, matrix const& rhs);
        matrix operator*(matrix const& lhs, typename matrix::value_type rhs);
        matrix operator*(typename matrix::value_type lhs, matrix const& rhs);

        std::ostream& operator<<(std::ostream& os, matrix const& rhs);
	}
}
#endif