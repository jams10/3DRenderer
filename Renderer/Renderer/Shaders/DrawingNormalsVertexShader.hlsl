
#include "Common.hlsli"

/*
*   노말 벡터를 그리기 위해 사용하는 정점 쉐이더.
*/

cbuffer MeshConstants : register(b0)
{
    matrix world;
    matrix worldInvTranspose;
};

cbuffer DrawingNormalConstants : register(b1)
{
    float scale;
    float3 dummy;
}

PixelShaderInput main(NormalVertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f);

    // Normal 먼저 변환.
    float3 normal = mul(float4(input.normalModel, 0.0f), worldInvTranspose).xyz;
    normal = normalize(normal);
    // 정점 위치 변환.
    pos = mul(pos, world);
    float t = input.texcoord.x; // 0인 경우 노말 벡터 시작점. 1인 경우 노말 벡터 끝점을 의미.
    pos.xyz += normal * t * scale; // 노말 벡터 시작점, 끝점 여부에 따라 끝점을 노말 벡터 방향으로 이동 시켜줌.
    
    pos = mul(pos, view);
    pos = mul(pos, proj);
    output.posProj = pos;
    
    output.color = float3(1.0, 1.0, 0.0) * (1.0 - t) + float3(1.0, 0.0, 0.0) * t;
    
    return output;
}

