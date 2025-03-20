#include "rasterizer.hpp"

#include "image.hpp"

Void Rasterizer::draw_triangle(const FVector3& vertex1, const FVector3& vertex2, const FVector3& vertex3, Color color, Image &image)
{
    const FVector4 bounds = calculate_triangle_bounds(vertex1, vertex2, vertex3);

    IVector2 min = image.to_pixel_space(bounds.x, bounds.y);
    IVector2 max = image.to_pixel_space(bounds.z, bounds.w);

    for (Int32 y = min.y; y < max.y; ++y)
    {
        for (Int32 x = min.x; x <= max.x; ++x)
        {
            FVector3 pixelPosition = FVector3(image.to_normalized_space(x, y), 0.0f);
            if (calculate_line_side(vertex1, vertex2, pixelPosition) >= 0 &&
                calculate_line_side(vertex2, vertex3, pixelPosition) >= 0 &&
                calculate_line_side(vertex3, vertex1, pixelPosition) >= 0)
            {
                image.set_pixel(x, y, color);
            }
        }
    }
}

FVector4 Rasterizer::calculate_triangle_bounds(const FVector2& vertex1, const FVector2& vertex2, const FVector2& vertex3)
{
    FVector2 min = Math::min(Math::min(vertex1, vertex2), vertex3);
    FVector2 max = Math::max(Math::max(vertex1, vertex2), vertex3);

    min = Math::clamp(min, -1.0f, 1.0f);
    max = Math::clamp(max, -1.0f, 1.0f);

    return { min.x, min.y, max.x, max.y };
}

Float32 Rasterizer::calculate_line_side(const FVector3& a, const FVector3& b, const FVector3 &point)
{
    return (point.y - a.y) * (b.x - a.x) - (b.y - a.y) * (point.x - a.x);
}