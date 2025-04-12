#pragma once
#include "vector.hpp"
#include "matrix.hpp"

namespace Math
{
    constexpr Float32 EPSILON         = 0.00000095367431640625f;
    constexpr Float32 PI              = 3.1415926535897932384626433832795f;
    constexpr Float32 ONE_OVER_PI     = 1.0f / PI;
    constexpr Float32 ONE_TWO_OVER_PI = 1.0f / (2.0f * PI);

#pragma region MIN
    template <Numeric Type>
    Type min(Type element1, Type element2) noexcept
    {
        return element1 > element2 ? element2 : element1;
    }

    template <Numeric Type>
    Vector<Type, 2> min(const Vector<Type, 2> &element1, const Vector<Type, 2> &element2) noexcept
    {
        return { min(element1.x, element2.x),
                 min(element1.y, element2.y) };
    }

    template <Numeric Type>
    Vector<Type, 2> min(Type element1, const Vector<Type, 2> &element2) noexcept
    {
        return { min(element1, element2.x),
                 min(element1, element2.y) };
    }

    template <Numeric Type>
    Vector<Type, 3> min(const Vector<Type, 3> &element1, const Vector<Type, 3> &element2) noexcept
    {
        return { min(element1.x, element2.x),
                 min(element1.y, element2.y), 
                 min(element1.z, element2.z) };
    }

    template <Numeric Type>
    Vector<Type, 3> min(Type element1, const Vector<Type, 3> &element2) noexcept
    {
        return { min(element1, element2.x),
                 min(element1, element2.y), 
                 min(element1, element2.z) };
    }

    template <Numeric Type>
    Vector<Type, 4> min(const Vector<Type, 4> &element1, const Vector<Type, 4> &element2) noexcept
    {
        return { min(element1.x, element2.x),
                 min(element1.y, element2.y), 
                 min(element1.z, element2.z), 
                 min(element1.w, element2.w) };
    }

    template <Numeric Type>
    Vector<Type, 4> min(Type element1, const Vector<Type, 4> &element2) noexcept
    {
        return { min(element1, element2.x),
                 min(element1, element2.y), 
                 min(element1, element2.z), 
                 min(element1, element2.w) };
    }
#pragma endregion

#pragma region MAX
    template <Numeric Type>
    Type max(Type element1, Type element2) noexcept
    {
        return element1 < element2 ? element2 : element1;
    }

    template <Numeric Type>
    Vector<Type, 2> max(const Vector<Type, 2> &element1, const Vector<Type, 2> &element2) noexcept
    {
        return { max(element1.x, element2.x),
                 max(element1.y, element2.y) };
    }

    template <Numeric Type>
    Vector<Type, 2> max(Type element1, const Vector<Type, 2> &element2) noexcept
    {
        return { max(element1, element2.x),
                 max(element1, element2.y) };
    }

    template <Numeric Type>
    Vector<Type, 3> max(const Vector<Type, 3> &element1, const Vector<Type, 3> &element2) noexcept
    {
        return { max(element1.x, element2.x),
                 max(element1.y, element2.y),
                 max(element1.z, element2.z) };
    }

    template <Numeric Type>
    Vector<Type, 3> max(Type element1, const Vector<Type, 3> &element2) noexcept
    {
        return { max(element1, element2.x),
                 max(element1, element2.y),
                 max(element1, element2.z) };
    }

    template <Numeric Type>
    Vector<Type, 4> max(const Vector<Type, 4> &element1, const Vector<Type, 4> &element2) noexcept
    {
        return { max(element1.x, element2.x),
                 max(element1.y, element2.y),
                 max(element1.z, element2.z),
                 max(element1.w, element2.w) };
    }

    template <Numeric Type>
    Vector<Type, 4> max(Type element1, const Vector<Type, 4> &element2) noexcept
    {
        return { max(element1, element2.x),
                 max(element1, element2.y),
                 max(element1, element2.z),
                 max(element1, element2.w) };
    }
#pragma endregion

#pragma region CLAMP
    template <Numeric Type>
    Type clamp(Type value, Type minValue, Type maxValue) noexcept
    {
        return min(max(value, minValue), maxValue);
    }

    template <Numeric Type>
    Vector<Type, 2> clamp(const Vector<Type, 2> &values, const Vector<Type, 2> &minValues, const Vector<Type, 2> &maxValues) noexcept
    {
        return { clamp(values.x, minValues.x, maxValues.x),
                 clamp(values.y, minValues.y, maxValues.y) };
    }

    template <Numeric Type>
    Vector<Type, 2> clamp(const Vector<Type, 2> &values, Type &minValue, const Vector<Type, 2> &maxValues) noexcept
    {
        return { clamp(values.x, minValue, maxValues.x),
                 clamp(values.y, minValue, maxValues.y) };
    }

    template <Numeric Type>
    Vector<Type, 2> clamp(const Vector<Type, 2> &values, const Vector<Type, 2> &minValues, Type maxValue) noexcept
    {
        return { clamp(values.x, minValues.x, maxValue),
                 clamp(values.y, minValues.y, maxValue) };
    }


    template <Numeric Type>
    Vector<Type, 2> clamp(const Vector<Type, 2> &values, Type minValue, Type maxValue) noexcept
    {
        return { clamp(values.x, minValue, maxValue),
                 clamp(values.y, minValue, maxValue) };
    }

    template <Numeric Type>
    Vector<Type, 3> clamp(const Vector<Type, 3> &values, const Vector<Type, 3> &minValues, const Vector<Type, 3> &maxValues) noexcept
    {
        return { clamp(values.x, minValues.x, maxValues.x),
                 clamp(values.y, minValues.y, maxValues.y),
                 clamp(values.z, minValues.z, maxValues.z) };
    }

    template <Numeric Type>
    Vector<Type, 3> clamp(const Vector<Type, 3> &values, Type minValue, const Vector<Type, 3> &maxValues) noexcept
    {
        return { clamp(values.x, minValue, maxValues.x),
                 clamp(values.y, minValue, maxValues.y),
                 clamp(values.z, minValue, maxValues.z) };
    }

    template <Numeric Type>
    Vector<Type, 3> clamp(const Vector<Type, 3> &values, const Vector<Type, 3> &minValues, Type maxValue) noexcept
    {
        return { clamp(values.x, minValues.x, maxValue),
                 clamp(values.y, minValues.y, maxValue),
                 clamp(values.z, minValues.z, maxValue) };
    }

    template <Numeric Type>
    Vector<Type, 3> clamp(const Vector<Type, 3> &values, Type minValue, Type maxValue) noexcept
    {
        return { clamp(values.x, minValue, maxValue),
                 clamp(values.y, minValue, maxValue),
                 clamp(values.z, minValue, maxValue) };
    }

    template <Numeric Type>
    Vector<Type, 4> clamp(const Vector<Type, 4> &values, const Vector<Type, 4> &minValues, const Vector<Type, 4> &maxValues) noexcept
    {
        return { clamp(values.x, minValues.x, maxValues.x),
                 clamp(values.y, minValues.y, maxValues.y),
                 clamp(values.z, minValues.z, maxValues.z),
                 clamp(values.w, minValues.w, maxValues.w) };
    }

    template <Numeric Type>
    Vector<Type, 4> clamp(const Vector<Type, 4> &values, Type minValue, const Vector<Type, 4> &maxValues) noexcept
    {
        return { clamp(values.x, minValue, maxValues.x),
                 clamp(values.y, minValue, maxValues.y),
                 clamp(values.z, minValue, maxValues.z),
                 clamp(values.w, minValue, maxValues.w) };
    }

    template <Numeric Type>
    Vector<Type, 4> clamp(const Vector<Type, 4> &values, const Vector<Type, 4> &minValues, Type maxValue) noexcept
    {
        return { clamp(values.x, minValues.x, maxValue),
                 clamp(values.y, minValues.y, maxValue),
                 clamp(values.z, minValues.z, maxValue),
                 clamp(values.w, minValues.w, maxValue) };
    }

    template <Numeric Type>
    Vector<Type, 4> clamp(const Vector<Type, 4> &values, Type minValue, Type maxValue) noexcept
    {
        return { clamp(values.x, minValue, maxValue),
                 clamp(values.y, minValue, maxValue),
                 clamp(values.z, minValue, maxValue),
                 clamp(values.w, minValue, maxValue) };
    }
#pragma endregion

#pragma region LENGTH2
    template <Numeric Type>
    Type length2(const Vector<Type, 2> &element) noexcept
    {
        return element.x * element.x + element.y * element.y;
    }

    template <Numeric Type>
    Type length2(const Vector<Type, 3> &element) noexcept
    {
        return element.x * element.x + element.y * element.y + element.z * element.z;
    }

    template <Numeric Type>
    Type length2(const Vector<Type, 4> &element) noexcept
    {
        return element.x * element.x + element.y * element.y + element.z * element.z + element.w * element.w;
    }
#pragma endregion

#pragma region LENGTH
    template <Numeric Type>
    Type length(const Vector<Type, 2> &element) noexcept
    {
        return sqrt(element.x * element.x + element.y * element.y);
    }

    template <Numeric Type>
    Type length(const Vector<Type, 3> &element) noexcept
    {
        return sqrt(element.x * element.x + element.y * element.y + element.z * element.z);
    }

    template <Numeric Type>
    Type length(const Vector<Type, 4> &element) noexcept
    {
        return sqrt(element.x * element.x + element.y * element.y + element.z * element.z + element.w * element.w);
    }
#pragma endregion

#pragma region NORMALIZE
    template <Numeric Type>
    Vector<Type, 2> normalize(const Vector<Type, 2> &element) noexcept
    {
        Type invLength = Type(1) / length(element);

        return { element.x * invLength, element.y * invLength };
    }

    template <Numeric Type>
    Vector<Type, 3> normalize(const Vector<Type, 3> &element) noexcept
    {
        Type invLength = Type(1) / length(element);

        return { element.x * invLength, element.y * invLength, element.z * invLength };
    }

    template <Numeric Type>
    Vector<Type, 4> normalize(const Vector<Type, 4> &element) noexcept
    {
        Type invLength = Type(1) / length(element);

        return { element.x * invLength, element.y * invLength, element.z * invLength, element.w * invLength };
    }
#pragma endregion

#pragma region DOT
    template <Numeric Type>
    Type dot(const Vector<Type, 2> &element1, const Vector<Type, 2> &element2) noexcept
    {
        return element1.x * element2.x + 
               element1.y * element2.y;
    }

    template <Numeric Type>
    Type dot(const Vector<Type, 3> &element1, const Vector<Type, 3> &element2) noexcept
    {
        return element1.x * element2.x + 
               element1.y * element2.y + 
               element1.z * element2.z;
    }

    template <Numeric Type>
    Type dot(const Vector<Type, 4> &element1, const Vector<Type, 4> &element2) noexcept
    {
        return element1.x * element2.x + 
               element1.y * element2.y + 
               element1.z * element2.z + 
               element1.w * element2.w;
    }
#pragma endregion

#pragma region CROSS
    template <Numeric Type>
    Type cross(const Vector<Type, 2> &element1, const Vector<Type, 2> &element2) noexcept
    {
        return element1.x * element2.y -
               element2.x * element1.y;
    }

    template <Numeric Type>
    Vector<Type, 3> cross(const Vector<Type, 3> &element1, const Vector<Type, 3> &element2) noexcept
    {
        return { element1.y * element2.z - element2.y * element1.z,
                 element1.z * element2.x - element2.z * element1.x,
                 element1.x * element2.y - element2.x * element1.y };
    }
#pragma endregion

    template <Numeric Type>
    Matrix<Type, 4, 4> look_at(const Vector<Type, 3> &eye, const Vector<Type, 3> &center, const Vector<Type, 3> &up)
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
        result[3][2] =  dot(f, eye);

        result[3][3] = Type(1);

        return result;
    }

    template <Numeric Type>
    Matrix<Type, 4, 4> perspective(Type fovYRadians, Type aspectRatio, Type near, Type far)
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

    template<Numeric Type>
    Type to_radians(Type degrees)
    {
        return degrees * (PI / 180.0f);
    }

    template<Numeric Type>
    Type to_degrees(Type radians)
    {
        return radians * (180.0f / PI);
    }
}