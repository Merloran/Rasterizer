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

    Type &operator[](UInt64 index) const noexcept
    {
        return *(this + index);
    }
};

template <Numeric Type>
class Vector<Type, 2UI64>
{
public:
    Type x, y;

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
    Type x, y, z;

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
};

template <Numeric Type>
class Vector<Type, 4UI64>
{
public:
    Type x, y, z, w;

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
};

#pragma region SPDLOG
template <typename Type>
struct fmt::formatter<Vector<Type, 2UI64>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Vector<Type, 2UI64> &vec, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "({:.2f}, {:.2f})", vec.x, vec.y);
    }
};

template <typename Type>
struct fmt::formatter<Vector<Type, 3UI64>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Vector<Type, 2UI64> &vec, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "({:.2f}, {:.2f}, {:.2f})", vec.x, vec.y, vec.z);
    }
};

template <typename Type>
struct fmt::formatter<Vector<Type, 4UI64>>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Vector<Type, 2UI64> &vec, FormatContext &ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "({:.2f}, {:.2f}, {:.2f}, {:.2f})", vec.x, vec.y, vec.z, vec.w);
    }
};
#pragma endregion

using FVector2 = Vector<Float32, 2UI64>;
using FVector3 = Vector<Float32, 3UI64>;
using FVector4 = Vector<Float32, 4UI64>;

using IVector2 = Vector<Int32, 2UI64>;
using IVector3 = Vector<Int32, 3UI64>;
using IVector4 = Vector<Int32, 4UI64>;