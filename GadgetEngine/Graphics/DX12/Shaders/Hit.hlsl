#include "Common.hlsl"

struct STriVertex
{
    float3 vertex;
    float4 color;
};

StructuredBuffer<STriVertex> BTriVertex : register(t0);

cbuffer Colors : register(b0)
{
    float3 A;
    float3 B;
    float3 C;
}

[shader("closesthit")]
void ClosestHit(inout HitInfo payload, Attributes attrib)
{
    float3 barycentrics = float3(1.0f - attrib.barycentric.x - attrib.barycentric.y, attrib.barycentric.x, attrib.barycentric.y);
    uint vertId = 3 * PrimitiveIndex();

    float3 hitColor = A * barycentrics.x + B * barycentrics.y + C * barycentrics.z;

    payload.colorAndDistance = float4(hitColor, RayTCurrent());
}

[shader("closesthit")]
void PlaneClosestHit(inout HitInfo payload, Attributes attrib)
{
    //float3 barycentrics = float3(1.0f - attrib.barycentric.x - attrib.barycentric.y, attrib.barycentric.x, attrib.barycentric.y);
    float3 hitColor = float3(0.75f, 0.75f, 0.3f);
    
    payload.colorAndDistance = float4(hitColor, RayTCurrent());
}