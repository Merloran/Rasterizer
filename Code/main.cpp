#include "image.hpp"
#include "rasterizer.hpp"
#include "Utilities/camera.hpp"
#include "Utilities/transform.hpp"
#include "Utilities/utils.hpp"
#include "Utilities/vertex.hpp"

Int32 main()
{
    const UVector2 resolution = { 1024, 1024 };
    Image image{ Int32(resolution.x), Int32(resolution.y), Color::BLACK };
    Image normalBuffer{ Int32(resolution.x), Int32(resolution.y), Color::BLACK };
    DepthBuffer depth{ resolution.x, resolution.y, 1.0f };

    Transform sphereTransform{};
    sphereTransform.set_local_position({ 0.0f, -1.0f, -3.0f });

    Transform coneTransform{};
    coneTransform.set_local_position({ 0.0f, 1.0f, -3.0f });
    coneTransform.set_local_euler_degrees({ 30.0f, 0.0f, 0.0f });

    Transform torus1Transform{};
    torus1Transform.set_local_position({ -1.0f, 0.0f, -2.0f });
    torus1Transform.set_local_euler_degrees({ -45.0f, 0.0f, 0.0f });

    Transform torus2Transform{};
    torus2Transform.set_local_position({ 1.0f, 0.0f, -2.0f });
    torus2Transform.set_local_euler_degrees({ -45.0f, 0.0f, 0.0f });


    Camera camera;
    camera.initialize({ 0.0f, 0.0f, 1.0f },
                      60.0f,
                      Float32(resolution.x) / Float32(resolution.y));

    Rasterizer rasterizer;
    DynamicArray<Vertex> vertexes;
    DynamicArray<UInt32> indexes;
    UniformBuffer buffer;
    OutputBuffers outBuffers;
    outBuffers.depthBuffer = &depth;
    outBuffers.normalBuffer = &normalBuffer;
    outBuffers.colorBuffer = &image;
    //Point
    buffer.lights.emplace_back(FVector3{ 0.0f, 3.0f, -3.0f },
                               FVector4{ 1.0f, 0.9f, 0.2f, 1.0f },
                               50.0f);
             
    // SPOT
    // buffer.lights.emplace_back(FVector3{ 0.0f, 0.0f, 1.0f },
    //                            camera.get_forward(),
    //                            FVector4{ 0.2f, 1.0f, 0.3f, 1.0f },
    //                            Math::to_radians(1.0f),
    //                            Math::to_radians(20.0f),
    //                            50.0f);

    // Direct
    buffer.lights.emplace_back(Math::normalize(FVector3{ 0.0f, -1.0f, 0.0f }),
                               Color::WHITE);

    buffer.viewProjection = camera.get_projection() * camera.get_view();
    buffer.viewPosition = camera.get_position();
    buffer.isVertexLighting = false;

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_torus(vertexes, 
                                     indexes, 
                                     0.4f, 
                                     0.2f, 
                                     16, 
                                     8, 
                                     Math::to_color(FVector4{ 0.2f, 0.7f, 0.2f, 1.0f }));
    buffer.model = torus1Transform.get_local_matrix();
    rasterizer.draw_mesh(vertexes, indexes, outBuffers, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, 
                                     indexes, 
                                     0.4f, 
                                     16, 
                                     16, 
                                     Math::to_color(FVector4{ 0.8f, 0.0f, 0.2f, 1.0f }));
    buffer.model = sphereTransform.get_local_matrix();
    rasterizer.draw_mesh(vertexes, indexes, outBuffers, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_cone(vertexes, 
                                     indexes, 
                                     0.4f, 
                                     1.0f, 
                                     16, 
                                     Math::to_color(FVector4{ 0.5f, 0.0f, 0.8f, 1.0f }));
    buffer.model = coneTransform.get_local_matrix();
    rasterizer.draw_mesh(vertexes, indexes, outBuffers, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_torus(vertexes, 
                                     indexes, 
                                     0.4f, 
                                     0.2f, 
                                     16, 
                                     8, 
                                     Math::to_color(FVector4{ 0.2f, 0.7f, 0.2f, 1.0f }));
    buffer.model = torus2Transform.get_local_matrix();
    buffer.isVertexLighting = true;
    rasterizer.draw_mesh(vertexes, indexes, outBuffers, buffer);


    image.save_to_file("../Images/Result.png");
    normalBuffer.save_to_file("../Images/Normal.png");
    Image(depth).save_to_file("../Images/Depth.png");

    return 0;
}