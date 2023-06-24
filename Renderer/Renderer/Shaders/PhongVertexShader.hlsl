
#include "Common.hlsli"

/*
*   위치, 노말, 텍스쳐 좌표를 가지는 정점을 트랜스폼하는 간단한 정점 쉐이더.
*/

cbuffer MeshConstants : register(b0)
{
    matrix world;
    matrix worldInvTranspose;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    output.posModel = input.posModel;
    float4 pos = float4(input.posModel, 1.0f); // 아핀 공간.
    pos = mul(pos, world);
    output.posWorld = pos.xyz;
    pos = mul(pos, view);
    pos = mul(pos, proj);

    output.posProj = pos;
    float4 normal = float4(input.normalModel, 0.0f);
    output.normalWorld = mul(normal, worldInvTranspose).xyz;
    output.normalWorld = normalize(output.normalWorld);
    output.tangentWorld = input.tangentModel;
    output.texcoord = input.texcoord;
    output.color = input.color;
    
    return output;
}