#include "Common.hlsl"

struct STriVertex
{
    float3 vertex;
    float4 color;
};

StructuredBuffer<STriVertex> BTriVertex : register(t0);

[shader("closesthit")]
void ClosestHit(inout HitInfo payload, Attributes attrib)
{
    float3 barycentrics = float3(1.0f - attrib.barycentric.x - attrib.barycentric.y, attrib.barycentric.x, attrib.barycentric.y);
    uint vertId = 3 * PrimitiveIndex();

    float3 hitColor = (
        (BTriVertex[vertId + 0].color * barycentrics.x) +
        (BTriVertex[vertId + 1].color * barycentrics.y) +
        (BTriVertex[vertId + 2].color * barycentrics.z)
    ).xyz;

    payload.colorAndDistance = float4(hitColor, RayTCurrent());
}