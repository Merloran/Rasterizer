#include "image.hpp"

Int32 main()
{
    const Image image { 512, 512, Color::RED };
	image.save_to_file("Hehe.png");

	return 0;
}