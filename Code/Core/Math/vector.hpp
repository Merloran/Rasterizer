#pragma once
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric Type, UInt64 Size>
requires (Size > 1) // Just use a normal numeric Type like a normal person!
class Vector
{
    Array<Type, Size> elements;

    Vector()
    {
        elements.fill({});
    }

    Vector(Type value)
    {
        elements.fill(value);
    }

    Type operator[](UInt64 index) const noexcept
    {
        assert(index < Size);
        return *(reinterpret_cast<const Type *>(this) + index);
    }

    Type &operator[](UInt64 index) noexcept
    {
        assert(index < Size);
        return *(reinterpret_cast<Type *>(this) + index);
    }
};

template <Numeric Type>
class Vector<Type, 2UI64>
{
public:
    union
    {
        struct
        {
            Type x, y;
        };
        struct
        {
            Type u, v;
        };
    };

    Vector() noexcept
        : x{}
        , y{}
    {}

    Vector(Type valueX, Type valueY) noexcept
        : x{ valueX }
        , y{ valueY }
    {}

    Vector(Type value) noexcept
        : x{ value }
        , y{ value }
    {}

    Vector(const Vector<Type, 3UI64> &other) noexcept
        : x{ other.x }
        , y{ other.y }
    {}

    Vector(const Vector<Type, 4UI64> &other) noexcept
        : x{ other.x }
        , y{ other.y }
    {}

    Type operator[](UInt64 index) const noexcept
    {
        assert(index < 2UI64);
        return *(reinterpret_cast<const Type *>(this) + index);
    }

    Type &operator[](UInt64 index) noexcept
    {
        assert(index < 2UI64);
        return *(reinterpret_cast<Type *>(this) + index);
    }

    Vector operator-(const Vector & other) const noexcept
    {
        return { x - other.x,
                 y - other.y };
    }

    Void operator-=(const Vector & other) noexcept
    {
        x -= other.x;
        y -= other.y;
    }

    Vector operator+(const Vector & other) const noexcept
    {
        return { x + other.x,
                 y + other.y };
    }

    Void operator+=(const Vector & other) noexcept
    {
        x += other.x;
        y += other.y;
    }
};

template <Numeric Type>
class Vector<Type, 3UI64>
{
public:
    union
    {
        struct
        {
            Type x, y, z;
        };
        struct
        {
            Type u, v, w;
        };
    };

    Vector() noexcept
        : x{}
        , y{}
        , z{}
    {}

    Vector(Type valueX, Type valueY, Type valueZ) noexcept
        : x{ valueX }
        , y{ valueY }
        , z{ valueZ }
    {}

    Vector(Type value) noexcept
        : x{ value }
        , y{ value }
        , z{ value }
    {}

    Vector(const Vector<Type, 2UI64> &other, Type valueZ = {}) noexcept
        : x{ other.x }
        , y{ other.y }
        , z{ valueZ  }
    {}

    Vector(const Vector<Type, 4UI64> &other) noexcept
        : x{ other.x }
        , y{ other.y }
        , z{ other.z }
    {}

#pragma region OPERATORS
    Type operator[](UInt64 index) const noexcept
    {
        assert(index < 3UI64);
        return *(reinterpret_cast<const Type *>(this) + index);
    }

    Type &operator[](UInt64 index) noexcept
    {
        assert(index < 3UI64);
        return *(reinterpret_cast<Type *>(this) + index);
    }

    Vector operator-(const Vector &other) const noexcept
    {
        return { x - other.x,
                 y - other.y,
                 z - other.z };
    }

    Void operator-=(const Vector &other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    Vector operator+(const Vector &other) const noexcept
    {
        return { x + other.x,
                 y + other.y,
                 z + other.z };
    }

    Void operator+=(const Vector &other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    Vector operator*(Type scalar) const noexcept
    {
        return { x * scalar,
                 y * scalar,
                 z * scalar };
    }

    Void operator*=(Type scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    Vector operator/(Type scalar) const noexcept
    {
        return { x / scalar,
                 y / scalar,
                 z / scalar };
    }

    Void operator/=(Type scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }
#pragma endregion
};

template <Numeric Type>
class Vector<Type, 4UI64>
{
public:
    union
    {
        struct
        {
            Type x, y, z, w;
        };
        struct
        {
            Type u, v, s, t;
        };
    };

    Vector() noexcept
        : x{}
        , y{}
        , z{}
        , w{}
    {}

    Vector(Type valueX, Type valueY, Type valueZ, Type valueW) noexcept
        : x{ valueX }
        , y{ valueY }
        , z{ valueZ }
        , w{ valueW }
    {}

    Vector(Type value) noexcept
        : x{ value }
        , y{ value }
        , z{ value }
        , w{ value }
    {}

    Vector(const Vector<Type, 2UI64> &other, Type valueZ = {}, Type valueW = {}) noexcept
        : x{ other.x }
        , y{ other.y }
        , z{ valueZ  }
        , w{ valueW  }
    {}

    Vector(const Vector<Type, 3UI64> &other, Type valueW = {}) noexcept
        : x{ other.x }
        , y{ other.y }
        , z{ other.z }
        , w{ valueW  }
    {}

#pragma region OPERATORS
    Type operator[](UInt64 index) const noexcept
    {
        assert(index < 4UI64);
        return *(reinterpret_cast<const Type *>(this) + index);
    }

    Type &operator[](UInt64 index) noexcept
    {
        assert(index < 4UI64);
        return *(reinterpret_cast<Type *>(this) + index);
    }

    Vector operator-(const Vector &other) const noexcept
    {
        return { x - other.x,
                 y - other.y,
                 z - other.z,
                 w - other.w };
    }

    Void operator-=(const Vector &other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    Vector operator+(const Vector &other) const noexcept
    {
        return { x + other.x,
                 y + other.y,
                 z + other.z,
                 w + other.w };
    }

    Void operator+=(const Vector &other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    Vector operator/(Type scalar) const noexcept
    {
        return { x / scalar,
                 y / scalar,
                 z / scalar,
                 w / scalar };
    }

    Void operator/=(Type scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
    }
#pragma endregion
};

#pragma region SPDLOG
template <typename Type, UInt64 Size>
struct fmt::formatter<Vector<Type, Size>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Vector<Type, Size> &vector, FormatContext &ctx) const -> decltype(ctx.out())
    {

        auto out = ctx.out();
        fmt::format_to(out, "Vector<{}>: (", Size);
        for (UInt64 i = 0; i < Size; ++i)
        {
            if constexpr (std::is_floating_point_v<Type>)
            {
                fmt::format_to(out, "{:>8.4f}", vector[i]);
            } else
            {
                fmt::format_to(out, "{:>8}", vector[i]);
            }

            if (i < Size - 1)
            {
                fmt::format_to(out, ", ");
            }
        }
        fmt::format_to(out, ")");

        return out;
    }
};
#pragma endregion

using FVector2 = Vector<Float32, 2UI64>;
using FVector3 = Vector<Float32, 3UI64>;
using FVector4 = Vector<Float32, 4UI64>;

using IVector2 = Vector<Int32, 2UI64>;
using IVector3 = Vector<Int32, 3UI64>;
using IVector4 = Vector<Int32, 4UI64>;

using UVector2 = Vector<UInt32, 2UI64>;
using UVector3 = Vector<UInt32, 3UI64>;
using UVector4 = Vector<UInt32, 4UI64>;