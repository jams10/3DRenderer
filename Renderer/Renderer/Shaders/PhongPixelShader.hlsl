
#include "Common.hlsli"

/*
*   Blinn Phong 반사 모델을 사용해 라이팅을 계산하는 픽셀 쉐이더.
*/

Texture2D g_texture0 : register(t0);

cbuffer PixelConstantBuffer : register(b0)
{
    Material material;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return material.bUseTexture ? g_texture0.Sample(linearClampSampler, input.texcoord) : float4(material.diffuse, 1.0f);
}