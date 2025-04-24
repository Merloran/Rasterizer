#pragma once

enum class ELightType
{
    None,
    Directional,
    Point,
    Spot
};

struct Light
{
    ELightType type;

    FVector3 direction;
    FVector3 position;

    Float32 intensity;
    Float32 cutOff;
    Float32 outerCutOff;
    FVector4 color;

    Light(const FVector3 &lightDirection, const FVector4 lightColor)
    : type(ELightType::Directional)
    , direction(lightDirection)
    , position()
    , intensity()
    , cutOff()
    , outerCutOff()
    , color(lightColor)
    {}

    Light(const FVector3 &lightPosition, const FVector4 lightColor, Float32 lightIntensity)
    : type(ELightType::Point)
    , direction()
    , position(lightPosition)
    , intensity(lightIntensity)
    , cutOff()
    , outerCutOff()
    , color(lightColor)
    {}

    Light(const FVector3 &lightPosition, 
          const FVector3 &lightDirection,
          const FVector4 lightColor,
          Float32 innerCutOff, 
          Float32 outerCutOffValue, 
          Float32 lightIntensity)
    : type(ELightType::Spot)
    , direction(Math::normalize(lightDirection))
    , position(lightPosition)
    , intensity(lightIntensity)
    , cutOff(cos(innerCutOff))
    , outerCutOff(cos(outerCutOffValue))
    , color(lightColor)
    {}
};