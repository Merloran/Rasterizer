#pragma once
#include "vector.hpp"


template <Numeric Type, UInt64 Width, UInt64 Height>
class Matrix
{
private:
    Array<Type, Width * Height> elements;
    static constexpr UInt64 WIDTH = Width, HEIGHT = Height;

public:
    Matrix()
    {
        elements.fill(0);
    }

    Matrix(Type Value)
        requires (Height == Width)
    {
        elements.fill(0);
        for (UInt64 i = 0UI64; i < Width * Height; i += Width + 1)
        {
            elements[i] = Value;
        }
    }

    Type* operator[](UInt64 index)
    {
        return &elements[index * Width];
    }

    const Type* operator[](UInt64 index) const
    {
        return &elements[index * Width];
    }

    Matrix operator+(const Matrix& other)
    {
        Matrix result;
        for (UInt64 y = 0UI64; y < Height; ++y)
        {
            for (UInt64 x = 0UI64; x < Width; ++x)
            {
                result[y][x] = (*this)[y][x] + other[y][x];
            }
        }
        return result;
    }

// 	void operator+=(const Matrix& M)
// 	{
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				this->Elements[x][y] += M.Elements[x][y];
// 			}
// 		}
// 	}
//
// 	Matrix operator-()
// 	{
// 		Matrix Temp;
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				Temp[x][y] = -this->Elements[x][y];
// 			}
// 		}
// 		return Temp;
// 	}
//
// 	Matrix operator-(const Matrix& M)
// 	{
// 		Matrix Temp;
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				Temp[x][y] = M.Elements[x][y] - this->Elements[x][y];
// 			}
// 		}
// 		return Temp;
// 	}
//
// 	void operator-=(const Matrix& M)
// 	{
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				this->Elements[x][y] -= M.Elements[x][y];
// 			}
// 		}
// 	}
//
// 	Matrix operator*(const Type& S)
// 	{
// 		Matrix Temp;
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				Temp[x][y] = this->Elements[x][y] * S;
// 			}
// 		}
// 		return Temp;
// 	}
//
// 	void operator*=(const Type& S)
// 	{
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				this->Elements[x][y] *= S;
// 			}
// 		}
// 	}
//
    template <UInt64 OtherWidth>
    Matrix<Type, OtherWidth, Height> operator*(const Matrix<Type, OtherWidth, Width> & other)
    {
        Matrix<Type, OtherWidth, Height> result{};

        for (UInt64 y = 0UI64; y < OtherWidth; ++y)
        {
            for (UInt64 x = 0UI64; x < Height; ++x)
            {
                for (UInt64 i = 0UI64; i < Width; ++i)
                {
                    result[y][x] += (*this)[i][x] * other[y][i];
                }
            }
        }
        return result;
    }

    Vector<Type, Height> operator*(const Vector<Type, Width> &other)
    {
        Vector<Type, Height> result{};

        for (UInt64 y = 0UI64; y < Height; ++y)
        {
            for (UInt64 i = 0UI64; i < Width; ++i)
            {
                result[y] += (*this)[i][y] * other[i];
            }
        }

        return result;
    }
//
// 	void operator*=(const Matrix& M)
// 	{
// 		Matrix Temp;
//
// 		if (YSize != M.XSize)
// 		{
// 			std::cout << "Cannot multiply matrix " << XSize << "x" << YSize << " by matrix " << M.XSize << "x" << M.YSize << "!" << std::endl;
// 			return;
// 		}
//
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < M.YSize; ++y)
// 			{
// 				for (uint32 i = (uint32)0; i < YSize; ++i)
// 				{
// 					Temp[x][y] += this->Elements[x][i] * M.Elements[i][y];
// 				}
// 			}
// 		}
// 		*this = Temp;
// 	}
//
// 	void Transpose()
// 	{
// 		for (uint32 x = (uint32)0; x < XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < YSize; ++y)
// 			{
// 				if (y > x)
// 				{
// 					std::swap(Elements[x][y], Elements[y][x]);
// 				}
// 			}
// 		}
// 	}
//
// 	static Type Determinant(const Matrix& M)
// 	{
// 		if (M.XSize == 1)
// 		{
// 			return M.Elements[0][0];
// 		}
// 		Type Result = (Type)0;
// 		for (uint32 i = (uint32)0; i < M.XSize; ++i)
// 		{
// 			Result += (Type)(i % 2 ? -1 : 1) * M.Elements[i][(uint32)0] * Determinant(Reshape(M, i, (uint32)0));
// 		}
//
// 		return Result;
// 	}
//
// 	static Matrix Cofactor(const Matrix& M)
// 	{
// 		Matrix Result;
// 		for (uint32 x = (uint32)0; x < M.XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < M.XSize; ++y)
// 			{
// 				Result[x][y] = (Type)((x + y) % 2 ? -1 : 1) * Determinant(Reshape(M, x, y));
// 			}
// 		}
// 		return Result;
// 	}
//
// 	static Matrix Inverse(const Matrix& M)
// 	{
// 		Type det = Determinant(M);
// 		if (det == (Type)0)
// 		{
// 			std::cout << "Matrix is singular, cannot calculate inverse matrix" << std::endl;
// 			return Matrix();
// 		}
//
// 		Matrix TAD = Cofactor(M);
// 		TAD.Transpose();
//
// 		Matrix Result;
// 		for (uint32 x = (uint32)0; x < M.XSize; ++x)
// 		{
// 			for (uint32 y = (uint32)0; y < M.XSize; ++y)
// 			{
// 				Result[x][y] = TAD[x][y] / det;
// 			}
// 		}
//
// 		return Result;
// 	}
//
//
// 	/** Translate Matrix 4x4 by Vector3 */
// 	void Translate(const Vector3<Type>& V)
// 	{
// 		if (XSize != 4 || YSize != 4)
// 		{
// 			std::cout << "Incorrect Matrix size!" << std::endl;
// 			return;
// 		}
// 		Elements[0][3] = V.X;
// 		Elements[1][3] = V.Y;
// 		Elements[2][3] = V.Z;
// 	}
//
// 	/** Scale Matrix 4x4 by Vector3 */
// 	void Scale(const Vector3<Type>& V)
// 	{
// 		if (XSize != 4 || YSize != 4)
// 		{
// 			std::cout << "Incorrect Matrix size!" << std::endl;
// 			return;
// 		}
// 		Elements[0][0] *= V.X;
// 		Elements[1][1] *= V.Y;
// 		Elements[2][2] *= V.Z;
// 	}
//
// 	/** Rotate Matrix 4x4 about Vector3 */
// 	void Rotate(Type Degrees, Vector3<Type> V)
// 	{
// 		if (XSize != 4 || YSize != 4)
// 		{
// 			std::cout << "Incorrect Matrix size!" << std::endl;
// 			return;
// 		}
// 		Vector3<Type> Axis = V.Normalize();
//
// 		Type SinAngle = (Type)sin(Axis.DegToRad(Degrees));
// 		Type CosAngle = (Type)cos(Axis.DegToRad(Degrees));
// 		Type OneMinusCosAngle = (Type)1 - CosAngle;
//
// 		Elements[0][0] = (Axis.X * Axis.X + CosAngle * (1 - Axis.X * Axis.X));
// 		Elements[0][1] = (Axis.X * Axis.Y * OneMinusCosAngle - SinAngle * Axis.Z);
// 		Elements[0][2] = (Axis.X * Axis.Z * OneMinusCosAngle + SinAngle * Axis.Y);
//
// 		Elements[1][0] = (Axis.X * Axis.Y * OneMinusCosAngle + SinAngle * Axis.Z);
// 		Elements[1][1] = (Axis.Y * Axis.Y + CosAngle * (1 - Axis.Y * Axis.Y));
// 		Elements[1][2] = (Axis.Y * Axis.Z * OneMinusCosAngle - SinAngle * Axis.X);
//
// 		Elements[2][0] = (Axis.X * Axis.Z * OneMinusCosAngle - SinAngle * Axis.Y);
// 		Elements[2][1] = (Axis.Y * Axis.Z * OneMinusCosAngle + SinAngle * Axis.X);
// 		Elements[2][2] = (Axis.Z * Axis.Z + CosAngle * (1 - Axis.Z * Axis.Z));
// 	}
//
// 	/** Multiply matrix by Vector4 */
// 	Vector4<Type> operator*(const Vector4<Type>& V)
// 	{
// 		if (XSize != 4 || YSize != 4)
// 		{
// 			std::cout << "Incorrect Matrix size!" << std::endl;
// 			return Vector4<Type>();
// 		}
//
// 		Vector4<Type> Temp;
//
// 		Temp.X = Elements[0][0] * V.X + Elements[0][1] * V.Y + Elements[0][2] * V.Z + Elements[0][3] * V.W;
// 		Temp.Y = Elements[1][0] * V.X + Elements[1][1] * V.Y + Elements[1][2] * V.Z + Elements[1][3] * V.W;
// 		Temp.Z = Elements[2][0] * V.X + Elements[2][1] * V.Y + Elements[2][2] * V.Z + Elements[2][3] * V.W;
// 		Temp.W = Elements[3][0] * V.X + Elements[3][1] * V.Y + Elements[3][2] * V.Z + Elements[3][3] * V.W;
//
// 		return Temp;
// 	}
//
// private:
// 	static Matrix Reshape(const Matrix& M, uint32 Row, uint32 Col)
// 	{
// 		Type** Values = new Type*[M.XSize - 1];
// 		for (uint32 i = (uint32)0; i < M.XSize - 1; ++i)
// 		{
// 			Values[i] = new Type[M.YSize - 1];
// 		}
// 		for (uint32 x = (uint32)0, TempX = (uint32)0; x < M.XSize; ++x, ++TempX)
// 		{
// 			if (x == Row)
// 			{
// 				--TempX;
// 				continue;
// 			}
//
// 			for (uint32 y = (uint32)0, TempY = (uint32)0; y < M.XSize; ++y, ++TempY)
// 			{
// 				if (y == Col)
// 				{
// 					--TempY;
// 					continue;
// 				}
// 				Values[TempX][TempY] = M.Elements[x][y];
// 			}
// 		}
//
// 		Matrix Result(Values);
//
// 		Result.XSize = M.XSize - 1;
// 		Result.YSize = M.YSize - 1;
//
// 		return Result;
// 	}
//
// 	Type Round(Type Value, Type Precision = (Type)0.000001)
// 	{
// 		if (Precision == (Type)0)
// 		{
// 			Precision = (Type)1;
// 		}
// 		return std::round(Value / Precision) * Precision;
// 	}
};


#pragma region SPDLOG
template <Numeric Type, UInt64 Width, UInt64 Height>
struct fmt::formatter<Matrix<Type, Width, Height>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Matrix<Type, Width, Height> &matrix, FormatContext &ctx) const -> decltype(ctx.out())
    {
        auto out = ctx.out();
        fmt::format_to(out, "Matrix<{}, {}>:\n", Width, Height);

        for (UInt64 y = 0; y < Height; ++y)
        {
            fmt::format_to(out, "[");
            for (UInt64 x = 0; x < Width; ++x)
            {
                if constexpr (std::is_floating_point_v<Type>)
                {
                    fmt::format_to(out, "{:>8.4f}", matrix[y][x]);
                }
                else
                {
                    fmt::format_to(out, "{:>8}", matrix[y][x]);
                }

                if (x < Width - 1)
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



using FMatrix4 = Matrix<Float32, 4UI64, 4UI64>;
using FMatrix3 = Matrix<Float32, 3UI64, 3UI64>;