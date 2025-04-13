#include "image.hpp"
#include "rasterizer.hpp"
#include "Utilities/camera.hpp"
#include "Utilities/vertex.hpp"

Int32 main()
{
    const UVector2 resolution = { 512, 512 };
    Image image { Int32(resolution.x), Int32(resolution.y), Color::BLACK };
    DepthBuffer depth { resolution.x, resolution.y, 1.0f };

    FMatrix4 modelMatrix = Math::translate(FMatrix4{ 1.0f }, 
                                          { 0.0f, 0.0f, 4.0f });
    modelMatrix = Math::rotate(modelMatrix, 
                               Math::to_degrees(45.0f),
                               {1.0f, 0.0f, 0.0f });

    modelMatrix = Math::rotate(modelMatrix, 
                               Math::to_degrees(45.0f),
                               {0.0f, 1.0f, 0.0f });
    modelMatrix = Math::scale(modelMatrix, { 0.5f });

    Camera camera;
    camera.initialize({ 0.0f },40.0f, 1.0f);
    Rasterizer rasterizer;


    // UV MAP        Y
    //	    ____     + X
    //     | TT |
    //     |    |
    //	   | BB |
    // ____|    |____
    //| LL   BT   RR |
    //|____      ____|
    //     | FF |
    //     |____|

    DynamicArray<Vertex> vertexes =
    {
        // Front face
        { { -1.0f, -1.0f, -1.0f }, Color::BLUE  }, // Left-Bottom
        { {  1.0f, -1.0f, -1.0f }, Color::RED   }, // Right-Bottom
        { {  1.0f,  1.0f, -1.0f }, Color::GREEN }, // Right-Top
        { { -1.0f,  1.0f, -1.0f }, Color::YELLOW}, // Left-Top

        // Back face
        { {  1.0f, -1.0f,  1.0f }, Color::BLUE  }, // Right-Bottom
        { { -1.0f, -1.0f,  1.0f }, Color::RED   }, // Left-Bottom
        { { -1.0f,  1.0f,  1.0f }, Color::GREEN }, // Left-Top
        { {  1.0f,  1.0f,  1.0f }, Color::YELLOW}, // Right-Top
    
        // Left face
        { { -1.0f, -1.0f,  1.0f }, Color::BLUE  }, // Back-Bottom
        { { -1.0f, -1.0f, -1.0f }, Color::RED   }, // Front-Bottom
        { { -1.0f,  1.0f, -1.0f }, Color::GREEN }, // Front-Top
        { { -1.0f,  1.0f,  1.0f }, Color::YELLOW}, // Back-Top
    
        // Right face
        { {  1.0f, -1.0f, -1.0f }, Color::BLUE  }, // Front-Bottom
        { {  1.0f, -1.0f,  1.0f }, Color::RED   }, // Back-Bottom
        { {  1.0f,  1.0f,  1.0f }, Color::GREEN }, // Back-Top
        { {  1.0f,  1.0f, -1.0f }, Color::YELLOW}, // Front-Top
    
        // Top face
        { { -1.0f,  1.0f, -1.0f }, Color::BLUE  }, // Front-Left
        { {  1.0f,  1.0f, -1.0f }, Color::RED   }, // Front-Right
        { {  1.0f,  1.0f,  1.0f }, Color::GREEN }, // Back-Right
        { { -1.0f,  1.0f,  1.0f }, Color::YELLOW}, // Back-Left
    
        // Bottom face
        { { -1.0f, -1.0f,  1.0f }, Color::BLUE  }, // Back-Left
        { {  1.0f, -1.0f,  1.0f }, Color::RED   }, // Back-Right
        { {  1.0f, -1.0f, -1.0f }, Color::GREEN }, // Top-Right
        { { -1.0f, -1.0f, -1.0f }, Color::YELLOW}, // Top-Left
    };
    
    DynamicArray<UInt32> indexes =
    {
         0,  1,  2,  2,  3,  0, // Front
         4,  5,  6,  6,  7,  4, // Back
         8,  9, 10, 10, 11,  8, // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20, // Bottom
    };


    rasterizer.draw_triangles(vertexes, 
                              indexes, 
                              image, 
                              depth, 
                              { modelMatrix, camera.get_view(), camera.get_projection() });
    image.save_to_file("../Images/Result.png");
    Image(depth).save_to_file("../Images/Depth.png");
    return 0;
}
