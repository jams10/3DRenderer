#include "Common.hlsli" // ���̴������� include ��� ����

TextureCube g_textureCube0 : register(t0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    return g_textureCube0.Sample(linearClampSampler, input.posWorld.xyz);
}