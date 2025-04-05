#pragma once

template <typename Type>
class Buffer
{
private:
    Type *elements;
    UInt64 width, height;

public:
    Buffer(const UInt64 bufferWidth, const UInt64 bufferHeight, const Type &value)
        : elements(new Type[bufferWidth * bufferHeight])
        , width(bufferWidth)
        , height(bufferHeight)
    {
        fill(value);
    }

    Buffer(Buffer &&source) noexcept
        : elements(source.elements)
        , width(source.width)
        , height(source.height)
    {
        source.elements = nullptr;
        source.~Buffer();
    }

    Buffer(const Buffer &source)
        : elements(new UInt8[source.width * source.height])
        , width(source.width)
        , height(source.height)
    {
        memcpy(elements, source.elements, source.width * source.height);
    }

    ~Buffer()
    {
        delete[] elements;
        elements = nullptr;
        width    = 0;
        height   = 0;
    }


    Void fill(const Type &element)
    {
        for (Type *current = elements, *end = elements + width * height;
             current < end;
             ++current)
        {
            memcpy(current, &element, sizeof(Type));
        }
    }

    Void set_element(const UInt64 x, const UInt64 y, const Type &element)
    {
        memcpy(&elements[x + y * width], &element, sizeof(Type));
    }

    [[nodiscard]]
    Type get_element(const UInt64 x, const UInt64 y) const
    {
        return elements[x + y * width];
    }

    [[nodiscard]]
    UVector2 to_grid_space(const Float32 x, const Float32 y) const
    {
        const UInt32 gridX = UInt32((x + 1.0f) * 0.5f * Float32(width - 1));
        const UInt32 gridY = UInt32((y + 1.0f) * 0.5f * Float32(height - 1));
        return { gridX, gridY };
    }

    [[nodiscard]]
    FVector2 to_normalized_space(const UInt64 x, const UInt64 y) const
    {
        const Float32 normalizedX = Float32(x) / Float32(width - 1) * 2.0f - 1.0f;
        const Float32 normalizedY = Float32(y) / Float32(height - 1) * 2.0f - 1.0f;
        return { normalizedX, normalizedY };
    }

    [[nodiscard]]
    Type *get_elements() const
    {
        return elements;
    }

    [[nodiscard]]
    UInt64 get_width() const
    {
        return width;
    }

    [[nodiscard]]
    UInt64 get_height() const
    {
        return height;
    }
};

using DepthBuffer = Buffer<Float32>;