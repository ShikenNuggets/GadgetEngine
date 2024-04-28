#include "Common.hlsl"

VSOutput TestShaderVS(float3 position : POSITION, float4 color : COLOR)
{
    VSOutput output;

    output.Position = float4(position.xyz, 1.0f);
    output.Color = color;
    
    return output;
}