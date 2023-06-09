
#include "Common.hlsli"

/*
*   Blinn Phong �ݻ� ���� ����� �������� ����ϴ� �ȼ� ���̴�.
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