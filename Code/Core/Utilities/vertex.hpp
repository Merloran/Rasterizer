#pragma once
#include "color.hpp"

struct Vertex
{
    FVector3 position;
    Color color;
};

struct FragmentVertex
{
    FVector4 position;
    Color color;
};