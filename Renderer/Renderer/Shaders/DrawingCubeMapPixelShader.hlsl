#include "Common.hlsli" // 쉐이더에서도 include 사용 가능

TextureCube g_textureCube0 : register(t0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    return g_textureCube0.Sample(linearClampSampler, input.posWorld.xyz);
}