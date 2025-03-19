#pragma once

struct Color
{
    UInt8 r, g, b, a;

    Color() = default;

    Color(const UInt8 initialValue) noexcept
    : r(initialValue)
    , g(initialValue)
    , b(initialValue)
    , a(255)
    {}

    explicit Color(const UInt32 hexColor) noexcept
    : r((hexColor & 0xFF000000) >> 24)
    , g((hexColor & 0x00FF0000) >> 16)
    , b((hexColor & 0x0000FF00) >> 8)
    , a( hexColor & 0x000000FF)
    {}

    Color(const UInt8 red, const UInt8 green, const UInt8 blue, const UInt8 alpha = 255) noexcept
    : r(red)
    , g(green)
    , b(blue)
    , a(alpha)
    {}

    constexpr UInt8 operator[](const UInt64 index) const noexcept
    {
        return *(&r + index);
    }

    explicit operator Int32()
    {
        return *reinterpret_cast<Int32 *>(&r);
    }

    Void print()
    {
        printf("%d %d %d %d", Int32(r), Int32(g), Int32(b), Int32(a));
    }

    static const Color RED  ;
    static const Color GREEN;
    static const Color BLUE ;
    static const Color BLACK;
    static const Color WHITE;
};