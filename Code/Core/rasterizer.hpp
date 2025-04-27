#pragma once
#include "buffer.hpp"
#include "Math/matrix.hpp"
#include "Utilities/light.hpp"

class Image;
struct FragmentInput;

struct UniformBuffer
{
    DynamicArray<Light> lights;
    FMatrix4 model;
    FMatrix4 viewProjection;
    FVector3 viewPosition;
    bool isVertexLighting = false;
};

struct OutputBuffers
{
    Image       *colorBuffer  = nullptr;
    Image       *normalBuffer = nullptr;
    Image       *uvBuffer     = nullptr;
    DepthBuffer *depthBuffer  = nullptr;
};

struct RasterizationInput;
class Camera;
struct Vertex;
struct Color;
enum class ELineSide : UInt8;
class Image;

class Rasterizer
{
public:
    Void draw_mesh(const DynamicArray<Vertex> &vertexes,
                   const DynamicArray<UInt32> &indexes,
                   const OutputBuffers &buffers,
                   const UniformBuffer &uniformBuffer);

    RasterizationInput process_vertex(const Vertex &vertex, const UniformBuffer &uniformBuffer);

    void process_fragment(const FragmentInput &fragment,
                          const OutputBuffers &buffers,
                          const UniformBuffer &uniformBuffer);

    Void draw_triangle(const RasterizationInput &vertex1,
                       const RasterizationInput &vertex2,
                       const RasterizationInput &vertex3,
                       const OutputBuffers &buffers,
                       const UniformBuffer &uniformBuffer);

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