#include "image.hpp"
#include "rasterizer.hpp"

Int32 main()
{
    Image image { 12, 12, Color::RED };

	Rasterizer rasterizer;
	FVector3 vertex1(-0.9f, 0.2f, 0.3f);
	FVector3 vertex2(0.6f, -0.7f, -0.4f);
	FVector3 vertex3(0.3f, 1.0f, -0.1f);
	rasterizer.draw_triangle(vertex1 - 0.5f, vertex2, vertex3, Color::WHITE, image);
	image.save_to_file("Hehe.png");
	return 0;
}
