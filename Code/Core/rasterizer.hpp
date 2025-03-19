#pragma once

struct Color;
enum class ELineSide : UInt8;
class Image;

class Rasterizer
{
public:
    Void draw_triangle(const FVector3 &vertex1,
                       const FVector3 &vertex2,
                       const FVector3 &vertex3,
                       Color color,
                       Image &image);

private:
    FVector4 calculate_triangle_bounds(const FVector2 &vertex1,
                                       const FVector2 &vertex2,
                                       const FVector2 &vertex3);

    Float32 calculate_line_side(const FVector3 &a, const FVector3 &b, const FVector3 &point);
};
