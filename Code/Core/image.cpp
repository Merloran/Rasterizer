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

Image::Image(Image&& source) noexcept
    : pixels(source.pixels)
    , width(source.width)
    , height(source.height)
{
    source.pixels = nullptr;
    source.~Image();
}

Image::Image(const Image& source)
    : pixels(new UInt8[source.width * source.height * sizeof(Color)])
    , width(source.width)
    , height(source.height)
{
    memcpy(pixels, source.pixels, source.width * source.height * sizeof(Color));
}

Image::~Image()
{
    delete[] pixels;
    pixels = nullptr;
    width  = 0;
    height = 0;
}

Void Image::fill(const Color color)
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
    memcpy(&pixels[(x + y * width) * sizeof(Color)], &color, sizeof(Color));
}

Color Image::get_pixel(const UInt64 x, const UInt64 y) const
{
    return *reinterpret_cast<Color*>(&pixels[(x + y * width) * sizeof(Color)]);
}

IVector2 Image::to_pixel_space(const Float32 x, const Float32 y) const
{
    const UInt64 pixelX = Int32((x + 1.0f) * 0.5f * Float32(width - 1));
    const UInt64 pixelY = Int32((y + 1.0f) * 0.5f * Float32(height - 1));
    return { pixelX, pixelY };
}

FVector2 Image::to_normalized_space(const Int32 x, const Int32 y) const
{
    const Float32 pixelX = Float32(x) / Float32(width - 1) * 2.0f - 1.0f;
    const Float32 pixelY = Float32(y) / Float32(height - 1) * 2.0f - 1.0f;
    return { pixelX, pixelY };
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
    stbi_flip_vertically_on_write(true);
    const Int32 result = stbi_write_png(filePath.c_str(), width, height, sizeof(Color), get_pixels(), 0);

    assert(result != 0 && "Failed to save path");
}

Void Image::load_from_file(const String& filePath)
{
    Int32 channels;
    pixels = stbi_load(filePath.c_str(), &width, &height, &channels, sizeof(Color));

    assert(channels == sizeof(Color) && "Channels count is invalid!");
}

Void Image::print()
{
    for (Int32 y = 0; y < height; ++y)
    {
        for (Int32 x = 0; x < width * sizeof(Color); ++x)
        {
            printf("%d ", Int32(pixels[x + y * width * sizeof(Color)]));
        }
        printf("\n");
    }
}
