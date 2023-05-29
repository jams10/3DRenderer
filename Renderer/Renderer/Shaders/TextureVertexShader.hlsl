
/*
*   위치, 노말, 텍스쳐 좌표를 가지는 정점을 트랜스폼하는 간단한 정점 쉐이더.
*/

cbuffer MVPMatrix : register(b0)
{
    matrix model;
    matrix invTranspose; // normal 벡터 트랜스폼 변환 용도.
    matrix view;
    matrix projection;
};

struct VertexShaderInput
{
    float3 posModel : POSITION;
    float3 normalModel : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 normalWorld : COLOR;
    float2 texcoord : TEXCOORD;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f); // 아핀 공간.
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.normalWorld = input.normalModel;
    output.texcoord = input.texcoord;
    
    return output;
}