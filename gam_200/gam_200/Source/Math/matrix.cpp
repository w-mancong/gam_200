#include "pch.h"
#include "Math/matrix.h"

namespace ManCong
{
    namespace Exception
    {
        /******************************************************************************************************************
                                           Invalid Deimensions
        ******************************************************************************************************************/
        InvalidDimension::InvalidDimension(long long rows, long long cols, const char* s) : msg{ "" }
        {
            const char* ex = "Invalid Dimension Exception: ", * err = "is an invalid dimension for ";
            std::ostringstream oss;
            if (s)
                oss << ex << rows << " and " << cols << " are invalid dimensions for " << s;
            else if (0 > rows && 0 > cols)
                oss << ex << rows << " and " << cols << " are invalid dimensions for rows and columns respectively";
            else if (0 > rows)
                oss << ex << rows << " " << err << "rows";
            else if (0 > cols)
                oss << ex << cols << " " << err << "columns";
            msg = oss.str();
        }

        const char* InvalidDimension::what(void) const noexcept
        {
            return msg.c_str();
        }

        /******************************************************************************************************************
                                                    Index Out of Bound
        ******************************************************************************************************************/
        IndexOutOfBounds::IndexOutOfBounds(long long row, long long R, long long col, long long C) : msg{ "" }
        {
            std::ostringstream oss;
            if ((0 > row || R <= row) && (0 > col || C <= col))
                oss << "Index Out Of Bounds Exception: " << row << " and " << col << " are invalid index for rows and columns respectively";
            else if (0 > row || R <= row)
                oss << "Index Out Of Bounds Exception: " << row << " is an invalid index for rows";
            else if (0 > col || C <= col)
                oss << "Index Out Of Bounds Exception: " << col << " is an invalid index for columns";
        }

        const char* IndexOutOfBounds::what(void) const noexcept
        {
            return msg.c_str();
        }

        /******************************************************************************************************************
                                                   Incompatible Matrices
        ******************************************************************************************************************/
        IncompatibleMatrices::IncompatibleMatrices(const char* operation, long long l_rows, long long l_cols, long long r_rows, long long r_cols) : msg{ "" }
        {
            std::ostringstream oss;
            oss << "Incompatible Matrices Exception: " << operation << "of LHS matrix with dimensions" << l_rows << " X " << l_cols << " and RHS matrix with dimensions " << r_rows << " X " << r_cols << " is undefined";
            msg = oss.str();
        }

        const char* IncompatibleMatrices::what(void) const noexcept
        {
            return msg.c_str();
        }
    }

	namespace Math
	{
        matrix::matrix(size_type R, size_type C) : mtx{ nullptr }, R{ R }, C{ C }
        {
            if (0 > R || 0 > C)
                throw Exception::InvalidDimension(R, C);
            mtx = Memory::MemoryManager<value_type>::GetInstance()->New(R, C);
            if (R == C)
                Indentity();
        }

        matrix::matrix(Vector2 const& rhs) : mtx{ nullptr }, R{ 3 }, C{ 1 }
        {
            mtx = Memory::MemoryManager<value_type>::GetInstance()->New(R, C);
            (*this)(0, 0) = rhs.x, (*this)(1, 0) = rhs.y, (*this)(2, 0) = 1.0f;
        }

        matrix::matrix(Vector3 const& rhs) : mtx{ nullptr }, R{ 4 }, C{ 1 }
        {
            mtx = Memory::MemoryManager<value_type>::GetInstance()->New(R, C);
            (*this)(0, 0) = rhs.x, (*this)(1, 0) = rhs.y, (*this)(2, 0) = rhs.z, (*this)(3, 0) = 1.0f;
        }

        matrix::~matrix(void)
        {
            if (mtx)
            {
                Memory::MemoryManager<value_type>::GetInstance()->Delete(mtx);
            }
        }

        matrix::matrix(matrix const& rhs) : mtx{ nullptr }, R{ rhs.R }, C{ rhs.C }
        {
            mtx = Memory::MemoryManager<value_type>::GetInstance()->New(R, C);
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                    (*this)(i, j) = rhs(i, j);
            }
        }

        matrix::matrix(matrix&& rhs) noexcept : mtx{ nullptr }, R{ 0 }, C{ 0 }
        {
            swap(rhs);
        }

        matrix& matrix::operator=(matrix const& rhs)
        {
            matrix tmp{ rhs };
            swap(tmp);
            return *this;
        }

        matrix& matrix::operator=(matrix&& rhs) noexcept
        {
            mtx = nullptr, R = 0, C = 0;
            swap(rhs);
            return *this;
        }

        matrix& matrix::operator=(Vector2 const& rhs)
        {
            matrix tmp{ rhs };
            swap(tmp);
            return *this;
        }

        matrix& matrix::operator=(Vector3 const& rhs)
        {
            matrix tmp{ rhs };
            swap(tmp);
            return *this;
        }

        matrix::reference matrix::operator()(size_type row, size_type col)
        {
            return const_cast<reference>(cget(row, col));
        }

        typename matrix::const_reference matrix::operator()(size_type row, size_type col) const
        {
            return cget(row, col);
        }

        bool matrix::operator==(matrix const& rhs) const
        {
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                {
                    if ((*this)(i, j) != rhs(i, j))
                        return false;
                }
            }
            return true;
        }

        bool matrix::operator!=(matrix const& rhs) const
        {
            return !(*this == rhs);
        }

        matrix& matrix::operator+=(matrix const& rhs)
        {
            if (R != rhs.R || C != rhs.C)
                throw Exception::IncompatibleMatrices("Addition", R, C, rhs.R, rhs.C);
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                    (*this)(i, j) += rhs(i, j);
            }
            return *this;
        }

        matrix& matrix::operator-=(matrix const& rhs)
        {
            if (R != rhs.R || C != rhs.C)
                throw Exception::IncompatibleMatrices("Subtraction", R, C, rhs.R, rhs.C);
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                    (*this)(i, j) -= rhs(i, j);
            }
            return *this;
        }

        matrix& matrix::operator*=(matrix const& rhs)
        {
            return *this = *this * rhs;
        }

        matrix& matrix::operator*=(value_type rhs)
        {
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                    (*this)(i, j) *= rhs;
            }
            return *this;
        }

        typename matrix::size_type matrix::Rows(void) const
        {
            return R;
        }

        typename matrix::size_type matrix::Cols(void) const
        {
            return C;
        }

        matrix& matrix::Transpose(void)
        {
            matrix tmp{ C, R };
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                    tmp(j, i) = (*this)(i, j);
            }
            swap(tmp);
            return *this;
        }

        matrix& matrix::Inverse(void)
        {
            if (R != C)
                throw Exception::InvalidDimension(R, C, "an inverse matrix. Must be a square matrix!");
            value_type det = Determinant(*this, R), flag = static_cast<value_type>(1);
            if (!det)
                return *this;
            matrix inv(R, C), tmp(R - 1, C - 1);
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                {
                    BarMatrix(tmp, *this, i, j);
                    inv(i, j) = Determinant(tmp, tmp.R) * flag;
                    flag *= -1.0f;
                }
            }
            inv *= static_cast<value_type>(1) / det;
            swap(inv);
            return *this;
        }

        void matrix::Indentity(void)
        {
            if (R != C)
                throw Exception::InvalidDimension(R, C, "an indentity matrix. Must be a square matrix!");
            for (size_type i = 0; i < R; ++i)
                (*this)(i, i) = static_cast<value_type>(1);
        }

        typename matrix::value_type matrix::Determinant(void) const
        {
            if (R != C)
                throw Exception::InvalidDimension(R, C, "finding determinant. Must be a square matrix!");
            return Determinant(*this, R);
        }

        typename matrix::value_type matrix::Determinant(matrix const& m, size_type n) const
        {
            if (n == 1)
                return m(0, 0);
            if (n == 2)
                return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
            value_type det = {};
            matrix tmp(n - 1, n - 1);
            for (size_type j = 0; j < m.C; ++j)
            {
                BarMatrix(tmp, m, 0, j);
                det += static_cast<value_type>(pow(-1.0, static_cast<double>(j))) * m(0, j) * Determinant(tmp, tmp.R);
            }
            return det;
        }

        void matrix::BarMatrix(matrix& dst, matrix const& src, size_type row, size_type col) const
        {
            for (size_type i = 0, r = 0; r < dst.R; ++i)
            {
                if (i == row)
                    continue;
                for (size_type j = 0, c = 0; c < dst.C; ++j)
                {
                    if (j == col)
                        continue;
                    dst(r, c++) = src(i, j);
                }
                ++r;
            }
        }

        matrix::const_reference matrix::cget(size_type row, size_type col) const
        {
            if (0 > row || R <= row || 0 > col || C <= col)
                throw Exception::IndexOutOfBounds(row, R, col, C);
            return *(mtx + row * C + col);
        }

        void matrix::swap(matrix& rhs)
        {
            std::swap(mtx, rhs.mtx);
            std::swap(R, rhs.R);
            std::swap(C, rhs.C);
        }

        matrix operator+(matrix const& lhs, matrix const& rhs)
        {
            matrix tmp{ lhs };
            tmp += rhs;
            return tmp;
        }

        matrix operator-(matrix const& lhs, matrix const& rhs)
        {
            matrix tmp{ lhs };
            tmp -= rhs;
            return tmp;
        }

        matrix operator*(matrix const& lhs, matrix const& rhs)
        {
            using size_type = typename matrix::size_type;
            const size_type l_rows = lhs.Rows(), l_cols = lhs.Cols(), r_rows = rhs.Rows(), r_cols = rhs.Cols();
            if (l_cols != r_rows)
                throw Exception::IncompatibleMatrices("Multiplication", l_rows, l_cols, r_rows, r_cols);
            matrix tmp(l_rows, r_cols);
            for (size_type i = 0; i < l_rows; ++i)
            {
                for (size_type j = 0; j < l_cols; ++j)
                {
                    for (size_type k = 0; k < r_cols; ++k)
                        tmp(i, k) += lhs(i, j) * rhs(j, k);
                }
            }
            return tmp;
        }

        matrix operator*(matrix const& lhs, typename matrix::value_type rhs)
        {
            matrix tmp{ lhs };
            return tmp *= rhs;
        }

        matrix operator*(typename matrix::value_type lhs, matrix const& rhs)
        {
            matrix tmp{ rhs };
            return tmp *= lhs;
        }

        std::ostream& operator<<(std::ostream& os, matrix const& rhs)
        {
            using size_type = typename matrix::size_type;
            const size_type R = rhs.Rows(), C = rhs.Cols();
            for (size_type i = 0; i < R; ++i)
            {
                for (size_type j = 0; j < C; ++j)
                    os << rhs(i, j) << (j + 1 == C ? '\0' : ' ');
                os << std::endl;
            }
            return os;
        }
	}
}