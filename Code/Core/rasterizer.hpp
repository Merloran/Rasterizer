#pragma once
#include "buffer.hpp"

struct FragmentVertex;
class Camera;
struct Vertex;
struct Color;
enum class ELineSide : UInt8;
class Image;

class Rasterizer
{
public:
    Void draw_triangles(const DynamicArray<Vertex> &vertexes,
                        const DynamicArray<UInt32> &indexes,
                        Image &image, 
                        DepthBuffer &depthBuffer, 
                        const Camera &camera);

    Void draw_triangle(const FragmentVertex &vertex1,
                       const FragmentVertex &vertex2,
                       const FragmentVertex &vertex3,
                       Image &image,
                       DepthBuffer &depthBuffer);

private:
    FVector4 calculate_triangle_bounds(const FVector2 &position1,
                                       const FVector2 &position2,
                                       const FVector2 &position3);

    Bool is_in_triangle(const FVector2 &position1,
                        const FVector2 &position2,
                        const FVector2 &position3,
                        const FVector2 &point);

    Float32 calculate_line_side(const FVector2 &a, const FVector2 &b, const FVector2 &point);

    Void calculate_barycentric(const FVector2 &vertex1,
                               const FVector2 &vertex2,
                               const FVector2 &vertex3,
                               const FVector2 &point,
                               FVector3       &result);
};