#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

Image::Image(const Int32 imageWidth, const Int32 imageHeight, Color initialColor)
    : pixels(new UInt8[imageHeight * imageWidth * sizeof(Color)])
    , width(imageWidth)
    , height(imageHeight)
{
    fill(initialColor);
}

Image::Image(const String& filePath)
{
    load_from_file(filePath);
}

Image::~Image()
{
    if (pixels != nullptr)
    {
        delete[] pixels;
        pixels = nullptr;
        width  = 0;
        height = 0;
    }
}

Void Image::fill(Color color)
{
    for (UInt8 *current = pixels, *end = pixels + width * height * sizeof(Color);
         current < end;
         current += sizeof(Color))
    {
        memcpy(current, &color, sizeof(Color));
    }
}

Void Image::set_pixel(const UInt64 x, const UInt64 y, const Color color)
{
    memcpy(&pixels[x + y * width], &color, sizeof(Color));
}

Color Image::get_pixel(UInt64 x, UInt64 y) const
{
    return pixels[x + y * width];
}

Color* Image::get_colors() const
{
    return reinterpret_cast<Color*>(pixels);
}

UInt8* Image::get_pixels() const
{
    return pixels;
}

UInt64 Image::get_width() const
{
    return width;
}

UInt64 Image::get_height() const
{
    return height;
}

Void Image::save_to_file(const String& filePath) const
{
    const Int32 result = stbi_write_png(filePath.c_str(), width, height, sizeof(Color), get_pixels(), 0);

    assert(result != 0 && "Failed to save path");
}

Void Image::load_from_file(const String& filePath)
{
    Int32 channels;
    pixels = stbi_load(filePath.c_str(), &width, &height, &channels, sizeof(Color));

    assert(channels == sizeof(Color) && "Channels count is invalid!");
}
