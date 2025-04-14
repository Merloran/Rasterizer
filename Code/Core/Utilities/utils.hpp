#pragma once

#include "vertex.hpp"

namespace GeometryGenerator
{
    inline Void generate_torus(DynamicArray<Vertex> &vertexes,
                               DynamicArray<UInt32> &indexes,
                               const Float32 majorRadius,
                               const Float32 minorRadius,
                               const UInt32 majorSegments,
                               const UInt32 minorSegments,
                               const Color &color)
    {
        assert(minorRadius <= majorRadius && majorSegments > 2UI32 && minorSegments > 1UI32);
        vertexes.reserve(majorSegments * minorSegments);
        indexes.reserve(majorSegments * minorSegments * 2UI32 * 3UI32);

        for (UInt32 i = 0UI32; i < majorSegments; ++i)
        {
            const Float32 phi = 2.0f * Math::PI * Float32(i) / Float32(majorSegments);
            const Float32 cosPhi = cos(phi);
            const Float32 sinPhi = sin(phi);

            const Float32 centerX = majorRadius * cosPhi;
            const Float32 centerY = 0.0f;
            const Float32 centerZ = majorRadius * sinPhi;

            for (UInt32 j = 0UI32; j < minorSegments; ++j)
            {
                const Float32 theta    = 2.0f * Math::PI * Float32(j) / Float32(minorSegments);
                const Float32 cosTheta = cos(theta);
                const Float32 sinTheta = sin(theta);

                const Float32 dirX = cosTheta * cosPhi;
                const Float32 dirY = sinTheta;
                const Float32 dirZ = cosTheta * sinPhi;

                Float32 x = centerX + minorRadius * dirX;
                Float32 y = centerY + minorRadius * dirY;
                Float32 z = centerZ + minorRadius * dirZ;

                vertexes.push_back(Vertex{ { x, y, z }, i % 2 ? j % 2 ? color : Color::GREEN : j % 2 ? Color::RED : Color::BLUE });
            }
        }

        for (UInt32 i = 0UI32; i < majorSegments; ++i)
        {
            const UInt32 i1 = i;
            const UInt32 i2 = (i + 1UI32) % majorSegments;

            for (UInt32 j = 0UI32; j < minorSegments; ++j)
            {
                const UInt32 j1 = j;
                const UInt32 j2 = (j + 1UI32) % minorSegments;

                UInt32 v1 = i1 * minorSegments + j1;
                UInt32 v2 = i2 * minorSegments + j1;
                UInt32 v3 = i2 * minorSegments + j2;
                UInt32 v4 = i1 * minorSegments + j2;

                indexes.push_back(v1);
                indexes.push_back(v2);
                indexes.push_back(v4);

                indexes.push_back(v2);
                indexes.push_back(v3);
                indexes.push_back(v4);
            }
        }
    }

    inline Void generate_uv_sphere(DynamicArray<Vertex> &vertexes,
                                   DynamicArray<UInt32> &indexes,
                                   const Float32 radius,
                                   const UInt32 rings,
                                   const UInt32 segments,
                                   const Color &color)
    {
        assert(rings > 1UI32 && segments > 1UI32);
        vertexes.reserve((rings + 1UI32) * (segments + 1UI32));
        indexes.reserve(rings * segments * 2UI32 * 3UI32);

        for (UInt32 r = 0UI32; r <= rings; ++r)
        {
            const Float32 phi = Math::PI * Float32(r) / Float32(rings) - Math::PI / 2.0f;
            Float32 y = radius * sin(phi);
            const Float32 cosPhiRadius = radius * cos(phi);

            for (UInt32 s = 0UI32; s <= segments; ++s)
            {
                const Float32 theta = 2.0f * Math::PI * Float32(s) / Float32(segments);
                Float32 x = cosPhiRadius * cos(theta);
                Float32 z = cosPhiRadius * sin(theta);

                vertexes.push_back(Vertex{ { x, y, z }, s % 2 ? r % 2 ? color : Color::RED : r % 2 ? Color::GREEN : Color::BLUE });
            }
        }

        for (UInt32 r = 0UI32; r < rings; ++r)
        {
            const UInt32 ringStart = r * (segments + 1UI32);
            const UInt32 nextRingStart = (r + 1UI32) * (segments + 1UI32);

            for (UInt32 s = 0UI32; s < segments; ++s)
            {
                UInt32 current = ringStart + s;
                UInt32 next = ringStart + s + 1UI32;
                UInt32 nextRingCurrent = nextRingStart + s;
                UInt32 nextRingNext = nextRingStart + s + 1UI32;

                // Square
                indexes.push_back(current);
                indexes.push_back(nextRingCurrent);
                indexes.push_back(next);

                indexes.push_back(next);
                indexes.push_back(nextRingCurrent);
                indexes.push_back(nextRingNext);
            }
        }
    }

    inline Void generate_cylinder(DynamicArray<Vertex> &vertexes,
                                  DynamicArray<UInt32> &indexes,
                                  const Float32 radius,
                                  const Float32 height,
                                  const UInt32 segments,
                                  const Color &topColor,
                                  const Color &bottomColor)
    {
        assert(segments > 1UI32);
        // Top Circle + Bottom Circle + Centers
        vertexes.reserve(segments * 2UI32 + 2UI32);
        indexes.reserve((segments * 4UI32) * 3UI32);

        // Top Circle center
        vertexes.push_back(Vertex{ { 0.0f, height, 0.0f }, topColor });

        // Bottom Circle center
        vertexes.push_back(Vertex{ { 0.0f }, bottomColor });

        for (UInt32 i = 0UI32; i < segments; ++i) // Top Circle
        {
            const Float32 angle = 2.0f * Math::PI * Float32(i) / Float32(segments);
            const Float32 x = radius * cos(angle);
            const Float32 z = radius * sin(angle);

            vertexes.push_back(Vertex{ { x, height, z }, i % 2 ? topColor : bottomColor });
        }

        for (UInt32 i = 0UI32; i < segments; ++i) // Bottom Circle
        {
            const Float32 angle = 2.0f * Math::PI * Float32(i) / Float32(segments);
            const Float32 x = radius * cos(angle);
            const Float32 z = radius * sin(angle);

            vertexes.push_back(Vertex{ { x, 0.0f, z }, i % 2 ? bottomColor : topColor });
        }

        for (UInt32 i = 0UI32; i < segments; ++i) // Top
        {
            UInt32 topCenterIndex = 0UI32;
            UInt32 current = 2UI32 + i;
            UInt32 next = 2UI32 + (i + 1UI32) % segments;

            indexes.push_back(topCenterIndex);
            indexes.push_back(current);
            indexes.push_back(next);
        }

        for (UInt32 i = 0UI32; i < segments; ++i) // Bottom
        {
            UInt32 bottomCenterIndex = 1UI32;
            UInt32 current = 2UI32 + segments + i;
            UInt32 next = 2UI32 + segments + (i + 1UI32) % segments;

            indexes.push_back(bottomCenterIndex);
            indexes.push_back(next);
            indexes.push_back(current);
        }

        for (UInt32 i = 0UI32; i < segments; ++i) // Side
        {
            UInt32 topCurrent    = 2UI32 + i;
            UInt32 topNext       = 2UI32 + (i + 1UI32) % segments;
            UInt32 bottomCurrent = 2UI32 + segments + i;
            UInt32 bottomNext    = 2UI32 + segments + (i + 1UI32) % segments;

            indexes.push_back(topCurrent);
            indexes.push_back(bottomCurrent);
            indexes.push_back(bottomNext);

            indexes.push_back(topCurrent);
            indexes.push_back(bottomNext);
            indexes.push_back(topNext);
        }
    }

    inline Void generate_cone(DynamicArray<Vertex> &vertexes,
                              DynamicArray<UInt32> &indexes,
                              const Float32 radius,
                              const Float32 height,
                              const UInt32 segments,
                              const Color &sideColor,
                              const Color &baseColor)
    {
        assert(segments > 1UI32);
        // Bottom Circle + Tip
        vertexes.reserve(segments + 2UI32);
        indexes.reserve(segments * 2UI32 * 3UI32);

        // Tip
        vertexes.push_back(Vertex{ { 0.0f, height, 0.0f }, sideColor });
        // Circle center
        vertexes.push_back(Vertex{ {}, baseColor });

        for (UInt32 i = 0UI32; i < segments; ++i) // Circle
        {
            const Float32 angle = 2.0f * Math::PI * Float32(i) / Float32(segments);
            const Float32 x = radius * cos(angle);
            const Float32 z = radius * sin(angle);

            vertexes.push_back(Vertex{ { x, 0.0f, z }, i % 2 ? baseColor : sideColor });
        }

        for (UInt32 i = 0; i < segments; ++i) // Side
        {
            UInt32 tipIndex = 0UI32;
            UInt32 current  = i + 2UI32;
            UInt32 next     = (i + 1UI32) % segments + 2UI32;

            indexes.push_back(tipIndex);
            indexes.push_back(current);
            indexes.push_back(next);
        }

        for (UInt32 i = 0UI32; i < segments; ++i) // Bottom
        {
            UInt32 centerIndex = 1UI32;
            UInt32 current     = i + 2UI32;
            UInt32 next        = (i + 1UI32) % segments + 2UI32;

            indexes.push_back(current);
            indexes.push_back(centerIndex);
            indexes.push_back(next);
        }
    }

    inline Void generate_cube(DynamicArray<Vertex> &vertexes,
                              DynamicArray<UInt32> &indexes)
    {
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

        vertexes =
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
        
        indexes =
        {
             0,  1,  2,  2,  3,  0, // Front
             4,  5,  6,  6,  7,  4, // Back
             8,  9, 10, 10, 11,  8, // Left
            12, 13, 14, 14, 15, 12, // Right
            16, 17, 18, 18, 19, 16, // Top
            20, 21, 22, 22, 23, 20, // Bottom
        };
    }

}
