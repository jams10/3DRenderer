
#include "Common.hlsli"

/*
*   위치와 색상만 가지는 정점을 트랜스폼하는 간단한 정점 쉐이더.
*/

cbuffer MeshConstants : register(b0)
{
    matrix world;
    matrix worldInvTranspose;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f); // 아핀 공간.
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);

    output.posProj = pos;
    output.color = input.color;
    
    return output;
}