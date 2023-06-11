
#include "Common.hlsli"

/*
*   보간되어 들어온 텍스쳐 좌표를 통해 텍스쳐를 샘플링, 샘플링 결과를 픽셀의 색상으로 사용하는 픽셀 쉐이더.
*/

Texture2D g_texture0 : register(t0);
    
cbuffer PixelConstantBuffer : register(b0)
{
    Material material;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return material.bUseTexture ? g_texture0.Sample(linearClampSampler, input.texcoord) : float4(material.color, 1.0f);
}