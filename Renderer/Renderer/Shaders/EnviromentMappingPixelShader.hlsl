#include "Common.hlsli" // 쉐이더에서도 include 사용 가능

TextureCube g_textureCube0 : register(t0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    // 1) 모델의 노말 방향으로 나아가서 큐브맵 텍스쳐를 샘플링 하는 방법.
    //return g_textureCube0.Sample(linearClampSampler, input.normalWorld.xyz);
    // 2) 시점 벡터가 모델의 노말 벡터와 만나 생기는 반사 벡터를 가지고 큐브맵 텍스쳐를 샘플링 하는 방법.
    // 주의할 점은 reflect 함수의 경우 시점에서 물체를 향하는 벡터를 입력으로 넣어야 물체 표면에서 반사되어 나가는 벡터를 구할 수 있음.
    float3 eyeToVertex = normalize(input.posWorld - eyeWorld);
    return g_textureCube0.Sample(linearClampSampler, reflect(eyeToVertex, input.normalWorld.xyz));
}