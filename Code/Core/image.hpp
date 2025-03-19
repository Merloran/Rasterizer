#pragma once
#include "Utilities/color.hpp"

class Image
{
private:
    UInt8* pixels;
    Int32 width, height;

public:
    Image(Int32 imageWidth, Int32 imageHeight, Color initialColor = Color::WHITE);
    Image(const String &filePath);

    ~Image();

    Void   fill(Color color);
    Void   set_pixel(UInt64 x, UInt64 y, Color color);

    [[nodiscard]]
    Color  get_pixel(UInt64 x, UInt64 y) const;

    Color *get_colors() const;
    UInt8 *get_pixels() const;

    [[nodiscard]]
    UInt64 get_width() const;
    [[nodiscard]]
    UInt64 get_height() const;

    Void save_to_file(const String &filePath) const;
    Void load_from_file(const String &filePath);
};

