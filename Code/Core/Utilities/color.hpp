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

    Color operator*(const Float32 value) const noexcept
    {
        return Color(r * value, g * value, b * value, a * value);
    }

    Color operator+(const Color other) const noexcept
    {
        return Color(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    explicit operator Int32()
    {
        return *reinterpret_cast<Int32 *>(&r);
    }

    static const Color RED  ;
    static const Color GREEN;
    static const Color BLUE ;
    static const Color BLACK;
    static const Color WHITE;
    static const Color YELLOW;
    static const Color MAGENTA;
    static const Color CYAN;
};


inline Color operator*(const Float32 value, const Color color)
{
    return Color(color.r * value, color.g * value, color.b * value, color.a * value);
}

template <>
struct fmt::formatter<Color>
{
    constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Color &color, FormatContext &ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "(R: {}, G: {}, B: {}, A: {})", color.r, color.g, color.b, color.a);
    }
};