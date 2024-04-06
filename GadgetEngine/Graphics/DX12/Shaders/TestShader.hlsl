struct VSOutput
{
    noperspective float4 Position : SV_POSITION;
    noperspective float2 UV : TEXCOORD;
};

VSOutput TestShaderVS(in uint VertexIdx : SV_VERTEXID)
{
    VSOutput output;

    float2 tex;
    float2 pos;
    
    if (VertexIdx == 0)
    {
        tex = float2(0.0f, 0.0f);
        pos = float2(0.0f, 0.5f);
    }
    else if (VertexIdx == 1)
    {
        tex = float2(0.0f, 2.0f);
        pos = float2(0.5f, -0.5f);
    }
    else //if (VertexIdx == 2)
    {
        tex = float2(2.0f, 0.0f);
        pos = float2(-0.5, -0.5f);
    }

    output.Position = float4(pos, 0.0f, 1.0f);
    output.UV = tex;
    
    return output;
}