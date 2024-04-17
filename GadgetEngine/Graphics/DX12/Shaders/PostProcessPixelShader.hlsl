struct PostProcessConstants
{
    uint GeometryPassMainBufferIndex;
};

ConstantBuffer<PostProcessConstants> ShaderParams : register(b1);

float4 PostProcessPS(in noperspective float4 Position : SV_POSITION, in noperspective float2 UV : TEXCOORD) : SV_Target0
{
    Texture2D geoPassMain = ResourceDescriptorHeap[ShaderParams.GeometryPassMainBufferIndex];
    return float4(geoPassMain[Position.xy].xyz, 1.0f);
}