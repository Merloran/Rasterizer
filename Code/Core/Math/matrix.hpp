#pragma once
#include "utilities.hpp"
#include "vector.hpp"

template <typename Type, typename ElementType, UInt64 Size>
concept VectorType = std::is_same_v<Type, Vector<ElementType, Size>>;

template <Numeric Type, UInt64 Columns, UInt64 Rows>
requires (Columns > 1 && Rows > 1)
class Matrix
{
public:
    static constexpr UInt64 COLUMNS = Columns, ROWS = Rows;
    Array<Type, Columns * Rows> elements;

    Matrix() noexcept
    {
        elements.fill(0);
    }

    Matrix(Type value) noexcept
        requires (Rows == Columns)
    {
        elements.fill(0);
        for (UInt64 i = 0UI64; i < Columns * Rows; i += Columns + 1)
        {
            elements[i] = value;
        }
    }

    template <typename... Args>
    requires (sizeof...(Args) == Columns * Rows)
    Matrix(Args... args) noexcept
    {
        UInt64 i = 0;
        ((
            elements[i] = static_cast<Type>(args),
            ++i
        ), ...);
    }

    template<VectorType<Type, Columns>... Args>
    requires (sizeof...(Args) == Rows)
    Matrix(const Args&... args) noexcept
    {
        UInt64 i = 0;
        ((
            (*this)[i] = args,
            ++i
        ), ...);
    }

#pragma region OPERATORS
    Vector<Type, Columns> &operator[](const UInt64 index) noexcept
    {
        assert(index < Rows);
        return *reinterpret_cast<Vector<Type, Columns> *>(&elements[index * Columns]);
    }

    const Vector<Type, Columns> &operator[](const UInt64 index) const noexcept
    {
        assert(index < Rows);
        return *reinterpret_cast<const Vector<Type, Columns> *>(&elements[index * Columns]);
    }

    Matrix operator+(const Matrix &other) const noexcept
    {
        Matrix result;
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[y][x] = (*this)[y][x] + other[y][x];
            }
        }
        return result;
    }

    Void operator+=(const Matrix &other) noexcept
    {
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                (*this)[y][x] += other[y][x];
            }
        }
    }

	Matrix operator-() const noexcept
	{
        Matrix result;
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[y][x] = -(*this)[y][x];
            }
        }
        return result;
	}

	Matrix operator-(const Matrix &other) const noexcept
	{
        Matrix result;
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[y][x] = (*this)[y][x] - other[y][x];
            }
        }
        return result;
	}

	Void operator-=(const Matrix &other) noexcept
	{
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                (*this)[y][x] -= other;
            }
        }
	}

    template <UInt64 OtherColumns> //TODO: Check it!!!
    Matrix<Type, OtherColumns, Rows> operator*(const Matrix<Type, OtherColumns, Columns> &other) const noexcept
    {
        Matrix<Type, OtherColumns, Rows> result{};

        for (UInt64 y = 0UI64; y < OtherColumns; ++y)
        {
            for (UInt64 x = 0UI64; x < Rows; ++x)
            {
                for (UInt64 i = 0UI64; i < Columns; ++i)
                {
                    result[y][x] += (*this)[i][x] * other[y][i];
                }
            }
        }
        return result;
    }

    Matrix operator*(Type scalar) const noexcept
    {
        Matrix result;

        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                    result[y][x] = (*this)[y][x] * scalar;
            }
        }
        return result;
    }

    Vector<Type, Columns> operator*(const Vector<Type, Rows> &other) const noexcept //TODO: Check it!
    {
        Vector<Type, Columns> result{};

        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[x] += (*this)[y][x] * other[y];
            }
        }

        return result;
    }

    Void operator*=(const Matrix &other) noexcept
    requires (Columns == Rows)
    {
        for (UInt64 y = 0UI64; y < Columns; ++y)
        {
            for (UInt64 x = 0UI64; x < Rows; ++x)
            {
                for (UInt64 i = 0UI64; i < Columns; ++i)
                {
                    (*this)[i][x] *= other[y][i];
                }
            }
        }
    }

    Void operator*=(Type scalar) noexcept
    requires (Columns == Rows)
    {
        for (UInt64 y = 0UI64; y < Columns; ++y)
        {
            for (UInt64 x = 0UI64; x < Rows; ++x)
            {
                (*this)[y][x] *= scalar;
            }
        }
    }
#pragma endregion
};


#pragma region SPDLOG
template <Numeric Type, UInt64 Columns, UInt64 Rows>
struct fmt::formatter<Matrix<Type, Columns, Rows>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Matrix<Type, Columns, Rows> &matrix, FormatContext &ctx) const -> decltype(ctx.out())
    {
        auto out = ctx.out();
        fmt::format_to(out, "Matrix<{}, {}>:\n", Columns, Rows);

        for (UInt64 y = 0; y < Rows; ++y)
        {
            fmt::format_to(out, "[");
            for (UInt64 x = 0; x < Columns; ++x)
            {
                if constexpr (std::is_floating_point_v<Type>)
                {
                    fmt::format_to(out, "{:>8.4f}", matrix[y][x]);
                }
                else
                {
                    fmt::format_to(out, "{:>8}", matrix[y][x]);
                }

                if (x < Columns - 1)
                {
                    fmt::format_to(out, ", ");
                }
            }
            fmt::format_to(out, "]\n");
        }

        return out;
    }
};
#pragma endregion


namespace Math
{
    template <Numeric Type, UInt64 Columns, UInt64 Rows>
    Matrix<Type, Rows, Columns> transpose(const Matrix<Type, Columns, Rows> &matrix) noexcept
    {
        Matrix<Type, Rows, Columns> result;
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[x][y] = matrix[y][x];
            }
        }
        return result;
    }

    template <Numeric Type>
    Type reshape(const Matrix<Type, 2UI64, 2UI64> &matrix,
                 const UInt64 rowToRemove,
                 const UInt64 columnToRemove) noexcept
    {
        assert(rowToRemove < 2UI64 && columnToRemove < 2UI64);
        return matrix[1UI64 - rowToRemove][1UI64 - columnToRemove];
    }

    template <Numeric Type, UInt64 Columns, UInt64 Rows>
    Matrix<Type, Columns - 1UI64, Rows - 1UI64> reshape(const Matrix<Type, Columns, Rows> &matrix,
                                                        const UInt64 rowToRemove,
                                                        const UInt64 columnToRemove) noexcept
        requires (Columns > 2UI64 && Rows > 2UI64)
    {
        assert(rowToRemove < Rows && columnToRemove < Columns);
        Matrix<Type, Columns - 1UI64, Rows - 1UI64> result;
        for (UInt64 matrixY = 0UI64, resultY = 0UI64; matrixY < Rows; ++matrixY, ++resultY)
        {
            if (matrixY == rowToRemove)
            {
                --resultY;
                continue;
            }

            for (UInt64 matrixX = 0UI64, resultX = 0UI64; matrixX < Columns; ++matrixX, ++resultX)
            {
                if (matrixX == columnToRemove)
                {
                    --resultX;
                    continue;
                }
                result[resultY][resultX] = matrix[matrixY][matrixX];
            }
        }

        return result;
    }

    template <Numeric Type>
    Type determinant(const Matrix<Type, 2UI64, 2UI64> &matrix) noexcept
    {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    template <Numeric Type, UInt64 Columns, UInt64 Rows>
    Type determinant(const Matrix<Type, Columns, Rows> &matrix) noexcept
    {
        Type result = Type(0);
        for (UInt64 i = 0UI64; i < Columns; ++i)
        {
            result += Type(i & 1UI64 ? -1 : 1)
                * matrix[i][0UI64]
                * determinant(reshape(matrix, i, 0UI64));
        }

        return result;
    }

    template <Numeric Type, UInt64 Columns, UInt64 Rows>
    Matrix<Type, Columns, Rows> cofactor(const Matrix<Type, Columns, Rows> &matrix) noexcept
    {
        Matrix<Type, Columns, Rows> result;
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[y][x] = Type((x + y) & 1UI64 ? -1 : 1)
                    * determinant(reshape(matrix, y, x));
            }
        }
        return result;
    }

    template <Numeric Type, UInt64 Columns, UInt64 Rows>
    Matrix<Type, Columns, Rows> inverse(const Matrix<Type, Columns, Rows> &matrix) noexcept
        requires (Columns == Rows)
    {
        Type det = determinant(matrix);
        assert(abs(det) > EPSILON && "Singular matrix cannot be inverted!");
        Type invDet = Type(1) / det;

        Matrix<Type, Columns, Rows> adjoint = cofactor(matrix);
        adjoint = transpose(adjoint);

        Matrix<Type, Columns, Rows> result;
        for (UInt64 y = 0UI64; y < Rows; ++y)
        {
            for (UInt64 x = 0UI64; x < Columns; ++x)
            {
                result[y][x] = adjoint[y][x] * invDet;
            }
        }

        return result;
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> look_at(const Vector<Type, 3> &eye,
                                       const Vector<Type, 3> &center,
                                       const Vector<Type, 3> &up) noexcept
    {
        Matrix<Type, 4, 4> result{};

        Vector<Type, 3> f = normalize(center - eye);
        Vector<Type, 3> s = normalize(cross(f, up));
        Vector<Type, 3> u = cross(s, f);

        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;

        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;

        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;

        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] = dot(f, eye);

        result[3][3] = Type(1);

        return result;
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> perspective(Type fovYRadians,
                                           Type aspectRatio,
                                           Type near,
                                           Type far) noexcept
    {
        Matrix<Type, 4, 4> result{};
        Type tanHalfFovY = tan(fovYRadians / Type(2));

        result[0][0] = Type(1) / (tanHalfFovY * aspectRatio);
        result[1][1] = Type(1) / tanHalfFovY;
        result[2][2] = far / (near - far);
        result[2][3] = -Type(1);
        result[3][2] = -(far * near) / (far - near);
        return result;
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> translate(const Matrix<Type, 4UI64, 4UI64> &matrix, 
                                         const Vector<Type, 3UI64> &vector) noexcept
    {
        Matrix<Type, 4UI64, 4UI64> result = matrix;

        result[3] = matrix[0] * vector[0]
                  + matrix[1] * vector[1]
                  + matrix[2] * vector[2]
                  + matrix[3];

        return result;
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> scale(const Matrix<Type, 4UI64, 4UI64> &matrix, 
                                     const Vector<Type, 3UI64> &vector) noexcept
    {
        Matrix<Type, 4UI64, 4UI64> result;

        result[0] = matrix[0] * vector[0];
        result[1] = matrix[1] * vector[1];
        result[2] = matrix[2] * vector[2];
        result[3] = matrix[3];

        return result;
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> rotate(const Matrix<Type, 4UI64, 4UI64> &matrix, 
                                      Type angleRadians, 
                                      const Vector<Type, 3UI64> &axis) noexcept
    {
        assert(length(axis) > EPSILON && "Axis should be normalized!");
    	Type sinAngle = sin(angleRadians);
    	Type cosAngle = cos(angleRadians);

        Vector<Type, 3UI64> temp((Type(1) - cosAngle) * axis);

        Matrix<Type, 4UI64, 4UI64> rotate;
        rotate[0][0] = cosAngle + temp.x * axis.x;
        rotate[0][1] = temp.x   * axis.y + sinAngle * axis.z;
        rotate[0][2] = temp.x   * axis.z - sinAngle * axis.y;

        rotate[1][0] = temp.y   * axis.x - sinAngle * axis.z;
        rotate[1][1] = cosAngle + temp.y * axis.y;
        rotate[1][2] = temp.y   * axis.z + sinAngle * axis.x;

        rotate[2][0] = temp.z   * axis.x + sinAngle * axis.y;
        rotate[2][1] = temp.z   * axis.y - sinAngle * axis.x;
        rotate[2][2] = cosAngle + temp.z * axis.z;

        Matrix<Type, 4UI64, 4UI64> result;
        result[0] = matrix[0] * rotate[0][0] + matrix[1] * rotate[0][1] + matrix[2] * rotate[0][2];
        result[1] = matrix[0] * rotate[1][0] + matrix[1] * rotate[1][1] + matrix[2] * rotate[1][2];
        result[2] = matrix[0] * rotate[2][0] + matrix[1] * rotate[2][1] + matrix[2] * rotate[2][2];
        result[3] = matrix[3];

        return result;
    }

    template <Numeric Type>
    Vector<Type, 3UI64> extract_translation(const Matrix<Type, 4UI64, 4UI64> &matrix) noexcept
    {
        return { matrix[3][0], matrix[3][1], matrix[3][2] };
    }

    template <Numeric Type>
    Vector<Type, 3UI64> extract_scale(const Matrix<Type, 4UI64, 4UI64> &matrix) noexcept
    {
        return { Math::length(matrix[0]), Math::length(matrix[1]), Math::length(matrix[2]) };
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> remove_scale(const Matrix<Type, 4UI64, 4UI64> &matrix) noexcept
    {
        Matrix<Type, 4UI64, 4UI64> result = matrix;

        result[0] = FVector4(normalize(FVector3(matrix[0])), 0.0f);
        result[1] = FVector4(normalize(FVector3(matrix[1])), 0.0f);
        result[2] = FVector4(normalize(FVector3(matrix[2])), 0.0f);

        return result;
    }
}


using FMatrix4 = Matrix<Float32, 4UI64, 4UI64>;
using FMatrix3 = Matrix<Float32, 3UI64, 3UI64>;