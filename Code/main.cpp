#include "image.hpp"
#include "rasterizer.hpp"
#include "Utilities/camera.hpp"
#include "Utilities/transform.hpp"
#include "Utilities/utils.hpp"
#include "Utilities/vertex.hpp"

Int32 main()
{
    const UVector2 resolution = { 512, 512 };
    Image image { Int32(resolution.x), Int32(resolution.y), Color::BLACK };
    DepthBuffer depth { resolution.x, resolution.y, 1.0f };

    Transform coneTransform{};
    coneTransform.set_local_position({ 0.0f, 0.0f, 3.0f });
    Transform cylinderTransform{};
    cylinderTransform.set_local_position({ 0.0f, -1.0f, 3.0f });
    Transform sphereTransform{};
    sphereTransform.set_local_position({ 0.0f, 2.5f, 3.0f });
    Transform torusTransform{};
    torusTransform.set_local_position({ 0.0f, 1.5f, 3.0f });
    torusTransform.set_local_scale({ 0.5f, 0.5f, 0.5f });

    Camera camera;
    camera.initialize({ 0.0f, 1.0f, 0.0f },
                      80.0f, 
                      1.0f);
    Rasterizer rasterizer;

    DynamicArray<Vertex> vertexes;
    DynamicArray<UInt32> indexes;

    // GeometryGenerator::generate_cube(vertexes, indexes);
    GeometryGenerator::generate_cone(vertexes, 
                                     indexes, 
                                     0.5f, 
                                     2.0f, 
                                     100, 
                                     Color::RED,
                                     Color::BLUE);
    rasterizer.draw_triangles(vertexes,
                              indexes,
                              image,
                              depth,
                              { coneTransform.get_local_matrix(),
                              camera.get_view(),
                              camera.get_projection() });

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_cylinder(vertexes,
                                         indexes, 
                                         0.5f, 
                                         1.0f, 
                                         16, 
                                         Color::RED, 
                                         Color::WHITE);
    rasterizer.draw_triangles(vertexes,
                              indexes,
                              image,
                              depth,
                              { cylinderTransform.get_local_matrix(),
                              camera.get_view(),
                              camera.get_projection() });

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_uv_sphere(vertexes,
                                          indexes, 
                                          0.5f, 
                                          256, 
                                          256, 
                                          Color::WHITE);
    rasterizer.draw_triangles(vertexes,
                              indexes,
                              image,
                              depth,
                              { sphereTransform.get_local_matrix(),
                              camera.get_view(),
                              camera.get_projection() });

    vertexes.clear(); indexes.clear();
    GeometryGenerator::generate_torus(vertexes,
                                      indexes,
                                      1.0f,
                                      0.5f,
                                      16,
                                      8,
                                      Color::BLACK);
    rasterizer.draw_triangles(vertexes,
                              indexes,
                              image,
                              depth,
                              { torusTransform.get_local_matrix(),
                              camera.get_view(),
                              camera.get_projection() });

    image.save_to_file("../Images/Result.png");
    Image(depth).save_to_file("../Images/Depth.png");
    return 0;
}
