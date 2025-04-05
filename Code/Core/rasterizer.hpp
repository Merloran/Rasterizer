#pragma once
#include "buffer.hpp"

struct Vertex;
struct Color;
enum class ELineSide : UInt8;
class Image;

class Rasterizer
{
public:
    Void draw_triangles(const DynamicArray<Vertex> &vertexes, Image &image, DepthBuffer &depthBuffer);

    Void draw_triangle(const Vertex &vertex1,
                       const Vertex &vertex2,
                       const Vertex &vertex3,
                       Image &image,
                       DepthBuffer &depthBuffer);

private:
    FVector4 calculate_triangle_bounds(const FVector2 &position1,
                                       const FVector2 &position2,
                                       const FVector2 &position3);

    Bool is_in_triangle(const FVector3 &position1,
                        const FVector3 &position2,
                        const FVector3 &position3,
                        const FVector3 &point);

    Float32 calculate_line_side(const FVector3 &a, const FVector3 &b, const FVector3 &point);

    Void calculate_barycentric(const FVector2 &vertex1,
                               const FVector2 &vertex2,
                               const FVector2 &vertex3,
                               const FVector2 &point,
                               FVector3       &result);
};