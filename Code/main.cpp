#include "image.hpp"
#include "rasterizer.hpp"
#include "Utilities/vertex.hpp"

Int32 main()
{
    Image image { 512, 512, Color::WHITE };

	Rasterizer rasterizer;
	Vertex v1 =
	{
		FVector3(-0.9f, 0.2f, 0.3f) - 0.4f,
		Color::RED
	};
	Vertex v2 =
	{
		FVector3(0.6f, -0.7f, -0.4f),
		Color::GREEN
	};
	Vertex v3 =
	{
		FVector3(0.3f, 1.0f, -0.1f),
		Color::BLUE
	};

	rasterizer.draw_triangle(v1, v2, v3, image);
	image.save_to_file("Hehe.png");
	return 0;
}
