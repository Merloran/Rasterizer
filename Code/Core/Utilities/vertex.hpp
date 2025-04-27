#pragma once

struct Vertex
{
    FVector3 position;
    FVector3 normal;
    FVector4 color;
};

struct RasterizationInput
{
    FVector4 position;
    FVector3 worldPosition;
    FVector3 worldNormal;
    FVector4 color;
};

struct FragmentInput
{
    FVector3 worldPosition;
    FVector3 worldNormal;
    FVector4 color;
    IVector2 position;
};