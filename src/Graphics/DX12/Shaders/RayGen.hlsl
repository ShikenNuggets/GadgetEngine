#include "Common.hlsl"

RWTexture2D<float4> gOutput : register(u0);

RaytracingAccelerationStructure SceneBVH : register(t0);

cbuffer RTCameraParams : register(b0)
{
    float4x4 viewInv;
    float4x4 projInv;
}

[shader("raygeneration")]
void RayGen()
{
    HitInfo payload;
    payload.colorAndDistance = float4(0.0f, 0.0f, 0.0f, 0.0f);

    uint2 launchIndex = DispatchRaysIndex().xy;
    float2 dims = float2(DispatchRaysDimensions().xy);
    float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.0f - 1.0f);
    
    float aspectRatio = dims.x / dims.y;
    float4 target = mul(projInv, float4(d.x, -d.y, 1.0f, 1.0f));

    RayDesc ray;
    ray.Origin = mul(viewInv, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
    ray.Direction = mul(viewInv, float4(target.xyz, 0.0f)).xyz;
    ray.TMin = 0;
    ray.TMax = 100000;

    TraceRay(SceneBVH, RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);

    gOutput[launchIndex] = float4(payload.colorAndDistance.rgb, 1.0f);
}