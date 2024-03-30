struct VSOutput{
    noperspective float4 Position : SV_POSITION;
    noperspective float2 UV : TEXCOORD;
};

VSOutput TestShaderVS(in uint VertexIdx : SV_VERTEXID){
    VSOutput output;

    output.Position = float4(0.0f, 0.0f, 0.0f, 1.0f);

    return output;
}