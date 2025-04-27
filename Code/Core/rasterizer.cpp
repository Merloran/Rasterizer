#include "rasterizer.hpp"

#include "image.hpp"
#include "Math/matrix.hpp"
#include "Utilities/vertex.hpp"

Void Rasterizer::draw_mesh(const DynamicArray<Vertex>& vertexes, const DynamicArray<UInt32> &indexes, const OutputBuffers &buffers, const UniformBuffer &uniformBuffer)
{
    assert(indexes.size() % 3 == 0 && "Triangles were not given!");
    assert(buffers.colorBuffer != nullptr && "Color buffer should be valid!");

    for (UInt64 i = 0; i < indexes.size(); i += 3)
    {
        draw_triangle(process_vertex(vertexes[indexes[i + 0]], uniformBuffer),
                      process_vertex(vertexes[indexes[i + 1]], uniformBuffer),
                      process_vertex(vertexes[indexes[i + 2]], uniformBuffer),
                      buffers,
                      uniformBuffer);
    }
}

RasterizationInput Rasterizer::process_vertex(const Vertex& vertex, const UniformBuffer &uniformBuffer)
{
    const FVector4 worldPosition = uniformBuffer.model * FVector4(vertex.position, 1.0f);
    const FVector4 clipPosition  = uniformBuffer.viewProjection * worldPosition;
    FVector3 normal = Math::normalize(FMatrix3(Math::transpose(Math::inverse(uniformBuffer.model))) * vertex.normal);

    if (uniformBuffer.isVertexLighting == false)
    {
        return { .position = clipPosition,
                 .worldPosition = worldPosition,
                 .worldNormal = normal,
                 .color = vertex.color };
    }

    FVector3 lightingColor = 0.0f;

    for (const Light &light : uniformBuffer.lights)
    {
        switch (light.type)
        {
            case ELightType::Directional:
            {
                Float32  ambientStrength = 0.1f;
                FVector3 ambient = FVector3(light.color) * ambientStrength;

                FVector3 diffuse = Math::max(Math::dot(normal, -light.direction), 0.0f) * FVector3(light.color) * 0.7f;

                Float32  specularStrength = 0.7f;
                FVector3 viewDir    = Math::normalize(uniformBuffer.viewPosition - FVector3(worldPosition));
                FVector3 reflectDir = Math::normalize(Math::reflect(light.direction, normal));
                FVector3 specular   = FVector3(light.color)
                                    * (pow(Math::max(Math::dot(viewDir, reflectDir), 0.0f), 32.0f)
                                    * specularStrength);

                lightingColor += ambient + diffuse + specular;
                break;
            }
            case ELightType::Point:
            {
                Float32  ambientStrength = 0.1f;
                FVector3 ambient = FVector3(light.color) * ambientStrength;

                FVector3 diffuse = Math::max(Math::dot(normal, -light.direction), 0.0f) * FVector3(light.color);
                
                Float32  specularStrength = 0.7f;
                FVector3 viewDir    = Math::normalize(uniformBuffer.viewPosition - FVector3(worldPosition));
                FVector3 lightDir   = Math::normalize(FVector3(worldPosition) - light.position);
                FVector3 reflectDir = Math::normalize(Math::reflect(lightDir, normal));
                FVector3 specular   = FVector3(light.color)
                                    * (pow(Math::max(Math::dot(viewDir, reflectDir), 0.0f), 32.0f)
                                    * specularStrength);

                Float32 distance = Math::length(light.position - FVector3(worldPosition));
                Float32 attenuation = light.intensity / (distance * distance + 0.0001f);

                ambient  *= attenuation;
                diffuse  *= attenuation;
                specular *= attenuation;

                lightingColor += ambient + diffuse + specular;
                break;
            }
            case ELightType::Spot:
            {
                Float32  ambientStrength = 0.1f;
                FVector3 ambient = FVector3(light.color) * ambientStrength;

                FVector3 diffuse = Math::max(Math::dot(normal, -light.direction), 0.0f) * FVector3(light.color);

                Float32  specularStrength = 0.7f;
                FVector3 viewDir = Math::normalize(uniformBuffer.viewPosition - FVector3(worldPosition));
                FVector3 reflectDir = Math::normalize(Math::reflect(light.direction, normal));
                FVector3 specular   = FVector3(light.color)
                                    * (pow(Math::max(Math::dot(viewDir, reflectDir), 0.0f), 32.0f)
                                    * specularStrength);


                FVector3 lightDir = Math::normalize(light.position - FVector3(worldPosition));
                Float32 theta     = Math::dot(lightDir, -light.direction);
                Float32 epsilon   = light.cutOff - light.outerCutOff;
                Float32 intensity = Math::clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

                Float32 distance    = Math::length(light.position - FVector3(worldPosition));
                Float32 attenuation = light.intensity / (distance * distance + 0.0001f);

                ambient  *= attenuation;
                diffuse  *= attenuation * intensity;
                specular *= attenuation * intensity;

                lightingColor += ambient + diffuse + specular;
                break;
            }
            case ELightType::None:
            break;
        }
    }

    FVector4 resultColor = FVector4(lightingColor, 1.0f) * vertex.color;

    return { .position = clipPosition,
             .worldPosition = worldPosition,
             .worldNormal = normal,
             .color = resultColor };
}

Void Rasterizer::draw_triangle(const RasterizationInput &vertex1, const RasterizationInput &vertex2, const RasterizationInput &vertex3, const OutputBuffers &buffers, const UniformBuffer &uniformBuffer)
{
    FVector3 ndcPosition1 = FVector3(vertex1.position) / vertex1.position.w;
    FVector3 ndcPosition2 = FVector3(vertex2.position) / vertex2.position.w;
    FVector3 ndcPosition3 = FVector3(vertex3.position) / vertex3.position.w;

    const FVector4 bounds = calculate_triangle_bounds(ndcPosition1, ndcPosition2, ndcPosition3);

    IVector2 min = buffers.colorBuffer->to_pixel_space(bounds.x, bounds.y);
    IVector2 max = buffers.colorBuffer->to_pixel_space(bounds.z, bounds.w);

    for (Int32 y = min.y; y <= max.y; ++y)
    {
        for (Int32 x = min.x; x <= max.x; ++x)
        {
            FVector2 pixelPosition = buffers.colorBuffer->to_normalized_space(x, y);
            if (is_in_triangle(ndcPosition1, 
                               ndcPosition2,
                               ndcPosition3, 
                               pixelPosition))
            {
                FVector3 barycentric;
                calculate_barycentric(ndcPosition1,
                                      ndcPosition2,
                                      ndcPosition3,
                                      pixelPosition, 
                                      barycentric);

                const Float32 depth = barycentric.u * ndcPosition1.z
                                    + barycentric.v * ndcPosition2.z
                                    + barycentric.w * ndcPosition3.z;
                
                if (buffers.depthBuffer == nullptr || depth < buffers.depthBuffer->get_element(x, y))
                {
                    const FVector4 color    = vertex1.color         * barycentric.u
                                            + vertex2.color         * barycentric.v
                                            + vertex3.color         * barycentric.w;

                    FVector3       normal   = vertex1.worldNormal   * barycentric.u
                                            + vertex2.worldNormal   * barycentric.v
                                            + vertex3.worldNormal   * barycentric.w;
                    normal = Math::normalize(normal);

                    const FVector3 position = vertex1.worldPosition * barycentric.u
                                            + vertex2.worldPosition * barycentric.v
                                            + vertex3.worldPosition * barycentric.w;

                    if (buffers.depthBuffer != nullptr)
                    {
                        buffers.depthBuffer->set_element(x, y, depth);
                    }

                    if (buffers.normalBuffer != nullptr)
                    {
                        buffers.normalBuffer->set_pixel(x, y, Math::to_color((normal + 1.0f) * 0.5f));
                    }

                    process_fragment({ .worldPosition = position, 
                                       .worldNormal = normal,
                                       .color = color,
                                       .position = { x, y } },
                                     buffers,
                                     uniformBuffer);
                }
            }
        }
    }
}

void Rasterizer::process_fragment(const FragmentInput &fragment, const OutputBuffers &buffers, const UniformBuffer &uniformBuffer)
{
    if (uniformBuffer.isVertexLighting)
    {
        buffers.colorBuffer->set_pixel(fragment.position.x, fragment.position.y, Math::to_color(fragment.color));
        return;
    }

    FVector3 lightingColor = 0.0f;

    const FVector3 &worldNormal = fragment.worldNormal;
    const FVector3 &worldPosition = fragment.worldPosition;

    for (const Light &light : uniformBuffer.lights)
    {
        switch (light.type)
        {
            case ELightType::Directional:
            {
                Float32  ambientStrength = 0.1f;
                FVector3 ambient = FVector3(light.color) * ambientStrength;

                FVector3 diffuse = Math::max(Math::dot(worldNormal, -light.direction), 0.0f) * FVector3(light.color) * 0.7f;

                Float32  specularStrength = 0.7f;
                FVector3 viewDir    = Math::normalize(uniformBuffer.viewPosition - worldPosition);
                FVector3 reflectDir = Math::normalize(Math::reflect(light.direction, worldNormal));
                FVector3 specular   = FVector3(light.color)
                                    * (pow(Math::max(Math::dot(viewDir, reflectDir), 0.0f), 32.0f)
                                    * specularStrength);

                lightingColor += ambient + diffuse + specular;
                break;
            }
            case ELightType::Point:
            {
                Float32  ambientStrength = 0.1f;
                FVector3 ambient = FVector3(light.color) * ambientStrength;

                FVector3 diffuse = Math::max(Math::dot(worldNormal, -light.direction), 0.0f) * FVector3(light.color);
                Float32  specularStrength = 0.7f;
                FVector3 viewDir = Math::normalize(uniformBuffer.viewPosition - worldPosition);
                FVector3 lightDir   = Math::normalize(worldPosition - light.position);
                FVector3 reflectDir = Math::normalize(Math::reflect(lightDir, worldNormal));
                FVector3 specular   = FVector3(light.color)
                                    * (pow(Math::max(Math::dot(viewDir, reflectDir), 0.0f), 32.0f)
                                    * specularStrength);

                Float32 distance = Math::length(light.position - worldPosition);
                Float32 attenuation = light.intensity / (distance * distance + 0.0001f);

                ambient  *= attenuation;
                diffuse  *= attenuation;
                specular *= attenuation;

                lightingColor += ambient + diffuse + specular;
                break;
            }
            case ELightType::Spot:
            {
                Float32  ambientStrength = 0.1f;
                FVector3 ambient = FVector3(light.color) * ambientStrength;

                FVector3 diffuse = Math::max(Math::dot(worldNormal, -light.direction), 0.0f) * FVector3(light.color);

                Float32  specularStrength = 0.7f;
                FVector3 viewDir = Math::normalize(uniformBuffer.viewPosition - worldPosition);
                FVector3 reflectDir = Math::normalize(Math::reflect(light.direction, worldNormal));
                FVector3 specular   = FVector3(light.color)
                                    * (pow(Math::max(Math::dot(viewDir, reflectDir), 0.0f), 32.0f)
                                    * specularStrength);


                FVector3 lightDir = Math::normalize(light.position - worldPosition);
                Float32 theta     = Math::dot(lightDir, -light.direction);
                Float32 epsilon   = light.cutOff - light.outerCutOff;
                Float32 intensity = Math::clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

                Float32 distance    = Math::length(light.position - worldPosition);
                Float32 attenuation = light.intensity / (distance * distance + 0.0001f);

                ambient  *= attenuation;
                diffuse  *= attenuation * intensity;
                specular *= attenuation * intensity;

                lightingColor += ambient + diffuse + specular;
                break;
            }
            case ELightType::None:
            break;
        }
    }

    FVector4 resultColor = FVector4(lightingColor, 1.0f) * fragment.color;

    buffers.colorBuffer->set_pixel(fragment.position.x, fragment.position.y, Math::to_color(resultColor));
}


FVector4 Rasterizer::calculate_triangle_bounds(const FVector2 &position1, const FVector2 &position2, const FVector2 &position3)
{
    FVector2 min = Math::min(Math::min(position1, position2), position3);
    FVector2 max = Math::max(Math::max(position1, position2), position3);

    min = Math::clamp(min, -1.0f, 1.0f);
    max = Math::clamp(max, -1.0f, 1.0f);

    return { min.x, min.y, max.x, max.y };
}

Bool Rasterizer::is_in_triangle(const FVector2 &position1, const FVector2 &position2, const FVector2 &position3, const FVector2 &point)
{
    const Float32 dx12 = position1.x - position2.x;
    const Float32 dx23 = position2.x - position3.x;
    const Float32 dx31 = position3.x - position1.x;
    const Float32 dy12 = position1.y - position2.y;
    const Float32 dy23 = position2.y - position3.y;
    const Float32 dy31 = position3.y - position1.y;

    const Bool isTopLeft1 = dy12 < 0 || (dy12 == 0.0f && dx12 > 0.0f);
    const Bool isTopLeft2 = dy23 < 0 || (dy23 == 0.0f && dx23 > 0.0f);
    const Bool isTopLeft3 = dy31 < 0 || (dy31 == 0.0f && dx31 > 0.0f);
    
    const Float32 edge1 = calculate_line_side(position1, position2, point);
    const Float32 edge2 = calculate_line_side(position2, position3, point);
    const Float32 edge3 = calculate_line_side(position3, position1, point);
    
    return (edge1 < 0.0f || (isTopLeft1 && edge1 <= 0.0f)) &&
           (edge2 < 0.0f || (isTopLeft2 && edge2 <= 0.0f)) &&
           (edge3 < 0.0f || (isTopLeft3 && edge3 <= 0.0f));
}

Float32 Rasterizer::calculate_line_side(const FVector2 &a, const FVector2 &b, const FVector2 &point)
{
    return (point.y - a.y) * (b.x - a.x) - (b.y - a.y) * (point.x - a.x); // Clock-wise
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