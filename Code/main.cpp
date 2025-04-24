#include "image.hpp"
#include "rasterizer.hpp"
#include "Utilities/camera.hpp"
#include "Utilities/transform.hpp"
#include "Utilities/utils.hpp"
#include "Utilities/vertex.hpp"

Int32 main()
{
    const UVector2 resolution = { 512, 512 };
    Image image{ Int32(resolution.x), Int32(resolution.y), Color::BLACK };
    DepthBuffer depth{ resolution.x, resolution.y, 1.0f };

    Transform sphereTransform{};
    sphereTransform.set_local_position({ 0.0f, 0.0f, -5.0f });
    sphereTransform.set_local_euler_degrees({ 0.0f, 0.0f, 0.0f });

    Transform sphere2Transform{};
    sphere2Transform.set_local_position({ -3.0f, 0.0f, -6.0f });
    sphere2Transform.set_local_euler_degrees({ 0.0f, 0.0f, 0.0f });

    Transform sphere3Transform{};
    sphere3Transform.set_local_position({ 3.0f, 0.0f, -4.0f });
    sphere3Transform.set_local_euler_degrees({ 0.0f, 0.0f, 0.0f });

    Transform smallSphere1Transform{};
    smallSphere1Transform.set_local_position({ -1.5f, -1.0f, -4.0f });

    Transform smallSphere2Transform{};
    smallSphere2Transform.set_local_position({ 1.5f, -1.0f, -6.0f });

    Transform smallSphere3Transform{};
    smallSphere3Transform.set_local_position({ 0.0f, -1.0f, -7.0f });

    Camera camera;
    camera.initialize({ 0.0f, 0.0f, 1.0f },
                      60.0f,
                      Float32(resolution.x) / Float32(resolution.y));

    Rasterizer rasterizer;
    DynamicArray<Vertex> vertexes;
    DynamicArray<UInt32> indexes;
    UniformBuffer buffer;
    //Point
    buffer.lights.emplace_back(FVector3{ 0.0f, 3.0f, -3.0f },
                               FVector4{ 1.0f, 0.9f, 0.2f, 1.0f },
                               50.0f);
             
    // SPOT
    buffer.lights.emplace_back(FVector3{ 0.0f, 0.0f, 1.0f },
                               camera.get_forward(),
                               FVector4{ 0.2f, 1.0f, 0.3f, 1.0f },
                               Math::to_radians(1.0f),
                               Math::to_radians(13.0f),
                               100.0f);

    // Direct
    buffer.lights.emplace_back(Math::normalize(FVector3{ 0.0f, -1.0f, 0.0f }),
                               Color::WHITE);

    buffer.viewPosition = camera.get_position();



    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, indexes, 1.5f, 32, 32, Math::to_color(FVector4{ 1.0f, 0.2f, 0.2f, 1.0f }));
    buffer.model = sphereTransform.get_local_matrix();
    buffer.viewProjection = camera.get_projection() * camera.get_view();
    rasterizer.draw_mesh(vertexes, indexes, image, depth, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, indexes, 1.2f, 32, 32, Math::to_color(FVector4{ 0.2f, 0.2f, 1.0f, 1.0f }));
    buffer.model = sphere2Transform.get_local_matrix();
    buffer.viewProjection = camera.get_projection() * camera.get_view();
    rasterizer.draw_mesh(vertexes, indexes, image, depth, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, indexes, 1.2f, 32, 32, Math::to_color(FVector4{ 0.2f, 1.0f, 0.2f, 1.0f }));
    buffer.model = sphere3Transform.get_local_matrix();
    buffer.viewProjection = camera.get_projection() * camera.get_view();
    rasterizer.draw_mesh(vertexes, indexes, image, depth, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, indexes, 0.5f, 20, 20, Math::to_color(FVector4{ 1.0f, 0.6f, 0.1f, 1.0f }));
    buffer.model = smallSphere1Transform.get_local_matrix();
    buffer.viewProjection = camera.get_projection() * camera.get_view();
    rasterizer.draw_mesh(vertexes, indexes, image, depth, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, indexes, 0.5f, 20, 20, Math::to_color(FVector4{ 0.8f, 0.2f, 0.8f, 1.0f }));
    buffer.model = smallSphere2Transform.get_local_matrix();
    buffer.viewProjection = camera.get_projection() * camera.get_view();
    rasterizer.draw_mesh(vertexes, indexes, image, depth, buffer);

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes, indexes, 0.5f, 20, 20, Math::to_color(FVector4{ 0.0f, 0.8f, 0.8f, 1.0f }));
    buffer.model = smallSphere3Transform.get_local_matrix();
    buffer.viewProjection = camera.get_projection() * camera.get_view();
    rasterizer.draw_mesh(vertexes, indexes, image, depth, buffer);

    image.save_to_file("../Images/Result.png");
    Image(depth).save_to_file("../Images/Depth.png");

    return 0;
}