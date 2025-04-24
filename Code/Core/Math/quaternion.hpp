#pragma once
#include "matrix.hpp"

template <Numeric Type>
class Quaternion
{
public:
    Type x, y, z, w;

    Quaternion() noexcept
        : x(Type(0))
        , y(Type(0))
        , z(Type(0))
        , w(Type(1))
    {}
    Quaternion(Type x, Type y, Type z, Type w) noexcept
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {}

    Quaternion(const Vector<Type, 3UI64> &axis, Type angleRadians) noexcept
    {
        Vector<Type, 3UI64> normAxis = Math::normalize(axis);
        Type halfAngle = angleRadians * Type(0.5);
        Type sinHalf   = std::sin(halfAngle);
        x = normAxis.x * sinHalf;
        y = normAxis.y * sinHalf;
        z = normAxis.z * sinHalf;
        w = std::cos(halfAngle);
    }

    Quaternion(const Vector<Type, 3UI64> &eulerRadians) noexcept
    {
        Type roll  = eulerRadians.x;
        Type pitch = eulerRadians.y;
        Type yaw   = eulerRadians.z;

        Type cr = std::cos(roll  * Type(0.5));
        Type sr = std::sin(roll  * Type(0.5));
        Type cp = std::cos(pitch * Type(0.5));
        Type sp = std::sin(pitch * Type(0.5));
        Type cy = std::cos(yaw   * Type(0.5));
        Type sy = std::sin(yaw   * Type(0.5));

        w = cr * cp * cy + sr * sp * sy;
        x = sr * cp * cy - cr * sp * sy;
        y = cr * sp * cy + sr * cp * sy;
        z = cr * cp * sy - sr * sp * cy;
    }

    Quaternion operator+(const Quaternion &other) const noexcept
    {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    Void operator+=(const Quaternion &other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    Quaternion operator-(const Quaternion &other) const noexcept
    {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    Quaternion operator-() const noexcept
    {
        return { -x, -y, -z, -w };
    }

    Void operator-=(const Quaternion &other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    Quaternion operator*(const Quaternion &other) const noexcept
    {
        const Vector<Type, 3UI64> selfImaginary  = imaginary();
        const Vector<Type, 3UI64> otherImaginary = other.imaginary();
        return { otherImaginary * w + selfImaginary * other.w + Math::cross(selfImaginary, otherImaginary),
                 w * other.w - Math::dot(selfImaginary, otherImaginary) };
    }

    Void operator*=(const Quaternion &other) noexcept
    {
        const Vector<Type, 3UI64> selfImaginary  = imaginary();
        const Vector<Type, 3UI64> otherImaginary = other.imaginary();

        imaginary() = otherImaginary * w
                    + selfImaginary * other.w
                    + Math::cross(selfImaginary, otherImaginary);

        w = w * other.w - Math::dot(selfImaginary, otherImaginary);
    }

    Quaternion operator/(const Quaternion &other) const noexcept
    {
        const Vector<Type, 3UI64> selfImaginary  = imaginary();
        const Vector<Type, 3UI64> otherImaginary = other.imaginary();

        Type denominator = w * other.w + Math::dot(otherImaginary, otherImaginary);
        assert(abs(denominator) > Math::EPSILON);
        Type invDenominator = Type(1) / denominator;

        return { (w * other.w + Math::dot(selfImaginary, otherImaginary)) * invDenominator,
                 (otherImaginary * -w + 
                  selfImaginary * other.w - Math::cross(selfImaginary,otherImaginary)) * invDenominator };
    }

    Void operator/=(const Quaternion &other) noexcept
    {
        const Vector<Type, 3UI64> selfImaginary = imaginary();
        const Vector<Type, 3UI64> otherImaginary = other.imaginary();

        Type denominator = w * other.w + Math::dot(otherImaginary, otherImaginary);
        assert(abs(denominator) > Math::EPSILON);
        Type invDenominator = Type(1) / denominator;
        
        imaginary() = (otherImaginary * -w +
                       selfImaginary * other.w - 
                       Math::cross(selfImaginary, otherImaginary)) * invDenominator;

        w = (w * other.w + Math::dot(selfImaginary, otherImaginary)) * invDenominator;
    }

    Quaternion operator/(Type scalar) const noexcept
    {
        return { x / scalar, y / scalar, z / scalar, w / scalar };
    }

    Void operator/=(Type scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
    }

    [[nodiscard]]
    Quaternion conjugate() const noexcept
    {
        return { -imaginary(), w };
    }

    Vector<Type, 3UI64> &imaginary() noexcept
    {
        return *reinterpret_cast<Vector<Type, 3UI64>*>(this);
    }

    [[nodiscard]]
    Vector<Type, 3UI64> imaginary() const noexcept
    {
        return *reinterpret_cast<const Vector<Type, 3UI64>*>(this);
    }
};

#pragma region SPDLOG
template <Numeric Type>
struct fmt::formatter<Quaternion<Type>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Quaternion<Type> &quaternion, FormatContext &ctx) const -> decltype(ctx.out())
    {

        if constexpr (std::is_floating_point_v<Type>)
        {
            return fmt::format_to(ctx.out(), 
                                  "Quaternion: ([{:>8.4f}, {:>8.4f}, {:>8.4f}], {:>8.4f})",
                                  quaternion.x, quaternion.y, quaternion.z, quaternion.w);
        }
        else
        {
            return fmt::format_to(ctx.out(), 
                                  "Quaternion: ([{:>8}, {:>8}, {:>8}], {:>8})",
                                  quaternion.x, quaternion.y, quaternion.z, quaternion.w);
        }
    }
};
#pragma endregion

namespace Math
{
    template <Numeric Type>
    Type length_squared(const Quaternion<Type> &quaternion) noexcept
    {
        return quaternion.x * quaternion.x +
               quaternion.y * quaternion.y +
               quaternion.z * quaternion.z +
               quaternion.w * quaternion.w;
    }

    template <Numeric Type>
    Type length(const Quaternion<Type> &quaternion) noexcept
    {
        return std::sqrt(quaternion.x * quaternion.x +
                         quaternion.y * quaternion.y +
                         quaternion.z * quaternion.z +
                         quaternion.w * quaternion.w);
    }

    template <Numeric Type>
    Quaternion<Type> normalize(const Quaternion<Type> &quaternion) noexcept
    {
        Type quaternionLength = length(quaternion);
        assert(abs(quaternionLength) > Math::EPSILON);
        Type invLength = Type(1) / quaternionLength;

        return { quaternion.x * invLength,
                 quaternion.y * invLength,
                 quaternion.z * invLength,
                 quaternion.w * invLength };
    }

    template <Numeric Type>
    Vector<Type, 3> rotate(const Vector<Type, 3> &vector, const Quaternion<Type> &quaternion) noexcept
    {
        Quaternion vectorQuaternion{ vector };
        Quaternion result = quaternion * vectorQuaternion * quaternion.conjugate();
        return result.imaginary();
    }

    template <Numeric Type>
    Quaternion<Type> inverse(const Quaternion<Type> &quaternion) noexcept
    {
        Type lengthSquared = length_squared(quaternion);
        assert(abs(lengthSquared) > Math::EPSILON);
        return quaternion.conjugate() / lengthSquared;
    }

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> to_matrix(const Quaternion<Type> &quaternion) noexcept
    {
        Type x = quaternion.x, y = quaternion.y, z = quaternion.z, w = quaternion.w;
        Type xx = x * x, yy = y * y, zz = z * z;
        Type xy = x * y, xz = x * z, yz = y * z;
        Type wx = w * x, wy = w * y, wz = w * z;

        Matrix<Type, 4UI64, 4UI64> result{};

        result[0][0] = Type(1) - Type(2) * (yy + zz);
        result[0][1] = Type(2) * (xy - wz);
        result[0][2] = Type(2) * (xz + wy);

        result[1][0] = Type(2) * (xy + wz);
        result[1][1] = Type(1) - Type(2) * (xx + zz);
        result[1][2] = Type(2) * (yz - wx);

        result[2][0] = Type(2) * (xz - wy);
        result[2][1] = Type(2) * (yz + wx);
        result[2][2] = Type(1) - Type(2) * (xx + yy);

        result[3][3] = Type(1);

        return result;
    }
    
    template <Numeric Type>
    Quaternion<Type> to_quaternion(const Matrix<Type, 4UI64, 4UI64>& matrix) noexcept
    {
        Type trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
        Quaternion<Type> quaternion;

        if (trace > Type(0))
        {
            Type s = std::sqrt(trace + Type(1)) * Type(2);
            assert(abs(s) > Math::EPSILON);
            Type invS = Type(1) / s;
            quaternion.w = Type(0.25) * s;
            quaternion.x = (matrix[2][1] - matrix[1][2]) * invS;
            quaternion.y = (matrix[0][2] - matrix[2][0]) * invS;
            quaternion.z = (matrix[1][0] - matrix[0][1]) * invS;
        }
        else if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2])
        {
            Type s = std::sqrt(Type(1) + matrix[0][0] - matrix[1][1] - matrix[2][2]) * Type(2);
            assert(abs(s) > Math::EPSILON);
            Type invS = Type(1) / s;
            quaternion.w = (matrix[2][1] - matrix[1][2]) * invS;
            quaternion.x = Type(0.25) * s;
            quaternion.y = (matrix[0][1] + matrix[1][0]) * invS;
            quaternion.z = (matrix[0][2] + matrix[2][0]) * invS;
        }
        else if (matrix[1][1] > matrix[2][2])
        {
            Type s = std::sqrt(Type(1) + matrix[1][1] - matrix[0][0] - matrix[2][2]) * Type(2);
            assert(abs(s) > Math::EPSILON);
            Type invS = Type(1) / s;
            quaternion.w = (matrix[0][2] - matrix[2][0]) * invS;
            quaternion.x = (matrix[0][1] + matrix[1][0]) * invS;
            quaternion.y = Type(0.25) * s;
            quaternion.z = (matrix[1][2] + matrix[2][1]) * invS;
        }
        else
        {
            Type s = std::sqrt(Type(1) + matrix[2][2] - matrix[0][0] - matrix[1][1]) * Type(2);
            assert(abs(s) > Math::EPSILON);
            Type invS = Type(1) / s;
            quaternion.w = (matrix[1][0] - matrix[0][1]) * invS;
            quaternion.x = (matrix[0][2] + matrix[2][0]) * invS;
            quaternion.y = (matrix[1][2] + matrix[2][1]) * invS;
            quaternion.z = Type(0.25) * s;
        }

        return quaternion;
    }

    template <Numeric Type>
    Vector<Type, 3> to_euler_radians(const Quaternion<Type> &quaternion) noexcept
    {
        Vector<Type, 3> euler;

        // Pitch
        Type sinP = Type(2) * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
        Type cosP = Type(1) - Type(2) * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
        euler[0] = atan2(sinP, cosP);

        // Yaw
        Type sinY = Type(2) * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
        if (abs(sinY) >= Type(1))
        {
            euler[1] = sinY >= 0 ? HALF_PI : -HALF_PI;
        }
        else
        {
            euler[1] = asin(sinY);
        }

        // Roll
        Type sinR = Type(2) * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
        Type cosR = Type(1) - Type(2) * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
        euler[2] = atan2(sinR, cosR);

        return euler;
    }

    template <Numeric Type>
    Quaternion<Type> extract_rotation(const Matrix<Type, 4UI64, 4UI64> &matrix)
    {
        const Vector<Type, 3UI64> invScale = Type(1) / extract_scale(matrix);

        const Matrix<Type, 4UI64, 4UI64> rotationMatrix =
        {
            Vector<Type, 4UI64>(Vector<Type, 3UI64>(matrix[0]) * invScale.x),
            Vector<Type, 4UI64>(Vector<Type, 3UI64>(matrix[1]) * invScale.y),
            Vector<Type, 4UI64>(Vector<Type, 3UI64>(matrix[2]) * invScale.z),
            Vector<Type, 4UI64>(Type(0), Type(0), Type(0), Type(1)),
        };

        const Quaternion<Type> result = to_quaternion(rotationMatrix);
        return result;
    }
    

    template <Numeric Type>
    Matrix<Type, 4UI64, 4UI64> orthonormal_multiply(const Matrix<Type, 4UI64, 4UI64> &parent, 
                                                    const Matrix<Type, 4UI64, 4UI64> &child)
    {
        const Vector<Type, 3UI64> parentScale = extract_scale(parent);
        const Matrix<Type, 4UI64, 4UI64> parentWithoutScale = remove_scale(parent);

        const Matrix<Type, 4UI64, 4UI64> localWithoutScale = translate({ 1.0f }, extract_translation(child))
                                                           * to_matrix(extract_rotation(child));

        Matrix<Type, 4UI64, 4UI64> orthonormalGlobal = parentWithoutScale * localWithoutScale;
        orthonormalGlobal = Math::scale(orthonormalGlobal, parentScale * scale);

        return orthonormalGlobal;
    }
}

using FQuaternion = Quaternion<Float32>;