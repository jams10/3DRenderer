
#include "Common.hlsli"

/*
*   �����Ǿ� ���� �ؽ��� ��ǥ�� ���� �ؽ��ĸ� ���ø�, ���ø� ����� �ȼ��� �������� ����ϴ� �ȼ� ���̴�.
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