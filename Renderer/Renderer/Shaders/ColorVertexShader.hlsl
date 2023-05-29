
/*
*   위치와 색상만 가지는 정점을 트랜스폼하는 간단한 정점 쉐이더.
*/

cbuffer MVPMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float3 color : COLOR0;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.pos, 1.0f); // 아핀 공간.
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.pos = pos;
    output.color = input.color;

    return output;
}