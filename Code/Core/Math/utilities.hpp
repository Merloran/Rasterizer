#pragma once
#include "vector.hpp"

namespace Math
{

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

#pragma region NORMALIZE
    //TODO: implement
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

}