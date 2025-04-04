#include "rasterizer.hpp"

#include "image.hpp"
#include "Utilities/vertex.hpp"

Void Rasterizer::draw_triangle(const Vertex &vertex1, const Vertex &vertex2, const Vertex &vertex3, Image &image)
{
    const FVector4 bounds = calculate_triangle_bounds(vertex1.position, vertex2.position, vertex3.position);

    IVector2 min = image.to_pixel_space(bounds.x, bounds.y);
    IVector2 max = image.to_pixel_space(bounds.z, bounds.w);

    for (Int32 y = min.y; y < max.y; ++y)
    {
        for (Int32 x = min.x; x <= max.x; ++x)
        {
            FVector3 pixelPosition = FVector3(image.to_normalized_space(x, y), 0.0f);
            if (is_in_triangle(vertex1.position, 
                               vertex2.position,
                               vertex3.position, 
                               pixelPosition))
            {
                FVector3 barycentric;
                calculate_barycentric(vertex1.position,
                                      vertex2.position,
                                      vertex3.position,
                                      pixelPosition, 
                                      barycentric);

                const Color color = barycentric.u * vertex1.color
                                  + barycentric.v * vertex2.color
                                  + barycentric.w * vertex3.color;

                image.set_pixel(x, y, color);
            }
        }
    }
}

FVector4 Rasterizer::calculate_triangle_bounds(const FVector2 &position1, const FVector2 &position2, const FVector2 &position3)
{
    FVector2 min = Math::min(Math::min(position1, position2), position3);
    FVector2 max = Math::max(Math::max(position1, position2), position3);

    min = Math::clamp(min, -1.0f, 1.0f);
    max = Math::clamp(max, -1.0f, 1.0f);

    return { min.x, min.y, max.x, max.y };
}

Bool Rasterizer::is_in_triangle(const FVector3& position1, const FVector3& position2, const FVector3& position3, const FVector3& point)
{
    return calculate_line_side(position1, position2, point) >= 0 &&
           calculate_line_side(position2, position3, point) >= 0 &&
           calculate_line_side(position3, position1, point) >= 0;
}

Float32 Rasterizer::calculate_line_side(const FVector3 &a, const FVector3 &b, const FVector3 &point)
{
    return (point.y - a.y) * (b.x - a.x) - (b.y - a.y) * (point.x - a.x);
}

Void Rasterizer::calculate_barycentric(const FVector2& vertex1, const FVector2& vertex2, const FVector2& vertex3, const FVector2& point, FVector3 & result)
{
    const FVector2 edge1 = vertex2 - vertex1;
    const FVector2 edge2 = vertex3 - vertex1;
    const FVector2 toPoint = point - vertex1;

    const Float32 d11 = Math::dot(edge1, edge1);
    const Float32 d12 = Math::dot(edge1, edge2);
    const Float32 d22 = Math::dot(edge2, edge2);
    const Float32 d31 = Math::dot(toPoint, edge1);
    const Float32 d32 = Math::dot(toPoint, edge2);

    const Float32 invDet = 1.0f / (d11 * d22 - d12 * d12);

    result.w = (d11 * d32 - d12 * d31) * invDet;
    result.v = (d22 * d31 - d12 * d32) * invDet;
    result.u = 1.0f - result.v - result.w;
}