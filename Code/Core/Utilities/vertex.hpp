#pragma once

struct Vertex
{
    FVector3 position;
    FVector3 normal;
    FVector4 color;
};

struct FragmentVertex
{
    FVector4 position;
    FVector3 normal;
    FVector4 color;
};