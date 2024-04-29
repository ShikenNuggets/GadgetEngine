struct HitInfo
{
    float4 colorAndDistance;
};

struct Attributes
{
    float2 barycentric;
};

struct VSOutput
{
    noperspective float4 Position : SV_POSITION;
    noperspective float4 Color : COLOR;
};

cbuffer RTCameraParams : register(b0)
{
    float4x4 viewInv;
    float4x4 projInv;
}