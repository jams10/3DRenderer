#include "Common.hlsli" // 쉐이더에서도 include 사용 가능

TextureCube g_textureCube0 : register(t0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    // 큐브맵은 원점에 위치하고 전체 씬을 감싸는 커다란 정육면체임.
    // 따라서 큐브맵 텍스쳐를 샘플링 할 때는 월드 좌표 원점을 중심으로 정육 면체를 구성하는 각 정점의 위치를 기준으로 샘플링 해주면 됨.
    // 월드 좌표가 샘플링 텍스쳐 좌표이므로 float3 타입.
    return g_textureCube0.Sample(linearClampSampler, input.posWorld.xyz);
}