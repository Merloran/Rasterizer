#include "image.hpp"
#include "rasterizer.hpp"
#include "Utilities/vertex.hpp"

Int32 main()
{

    const UVector2 resolution = { 512, 512 };
    Image image { Int32(resolution.x), Int32(resolution.y), Color::WHITE };
    DepthBuffer depth { resolution.x, resolution.y, 1.0f };

	Rasterizer rasterizer;
	DynamicArray<Vertex> vertexes;

	vertexes.emplace_back(FVector3(-0.5f,  0.5f,  0.8f), Color::BLUE);
	vertexes.emplace_back(FVector3(-0.5f, -0.5f,  0.8f), Color::RED);
	vertexes.emplace_back(FVector3( 0.5f, -0.5f,  0.8f), Color::GREEN);

	vertexes.emplace_back(FVector3(-0.5f,  0.5f,  0.8f), Color::BLUE);
	vertexes.emplace_back(FVector3( 0.5f, -0.5f,  0.8f), Color::GREEN);
	vertexes.emplace_back(FVector3( 0.5f,  0.5f,  0.8f), Color::RED);
    
	vertexes.emplace_back(FVector3(-0.6f, -0.8f,  0.9f), Color::MAGENTA);
	vertexes.emplace_back(FVector3( 0.6f,  0.5f, -0.3f), Color::CYAN);
    vertexes.emplace_back(FVector3( 0.0f,  0.2f,  0.9f), Color::YELLOW);

	rasterizer.draw_triangles(vertexes, image, depth);
	image.save_to_file("../Images/Result.png");
	Image(depth).save_to_file("../Images/Depth.png");
	return 0;
}
