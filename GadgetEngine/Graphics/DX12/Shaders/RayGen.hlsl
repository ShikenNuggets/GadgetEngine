#include "Common.hlsl"

RWTexture2D<float4> gOutput : register(u0);

RaytracingAccelerationStructure SceneBVH : register(t0);

[shader("raygeneration")]
void RayGen()
{
    HitInfo payload;
    payload.colorAndDistance = float4(0.0f, 0.0f, 0.0f, 0.0f);

    uint2 launchIndex = DispatchRaysIndex().xy;
    float2 dims = float2(DispatchRaysDimensions().xy);
    float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.0f - 1.0f);

    RayDesc ray;
    ray.Origin = float3(d.x, -d.y, 1.0f);
    ray.Direction = float3(0.0f, 0.0f, -1.0f);
    ray.TMin = 0;
    ray.TMax = 100000;

    TraceRay(SceneBVH, RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);

    gOutput[launchIndex] = float4(payload.colorAndDistance.rgb, 1.0f);
}