struct ShaderConstants
{
    float width;
    float height;
    uint frame;
};

ConstantBuffer<ShaderConstants> ShaderParams : register(b1);

float4 TestShaderPS(in noperspective float4 Position : SV_POSITION, in noperspective float2 UV : TEXCOORD) : SV_Target0{
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
}