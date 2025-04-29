#pragma once

struct Vertex
{
    FVector3 position;
    FVector3 normal;
    FVector2 uv;
};

struct RasterizationInput
{
    FVector4 position;
    FVector3 worldPosition;
    FVector3 worldNormal;
    FVector2 uv;
};

struct FragmentInput
{
    FVector3 worldPosition;
    FVector3 worldNormal;
    FVector2 uv;
    IVector2 position;
};