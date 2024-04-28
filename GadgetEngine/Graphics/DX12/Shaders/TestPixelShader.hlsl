#include "Common.hlsl"

float4 TestShaderPS(VSOutput input) : SV_TARGET
{
    return input.Color;
}