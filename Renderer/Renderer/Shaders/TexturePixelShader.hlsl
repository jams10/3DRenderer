
/*
*   보간되어 들어온 텍스쳐 좌표를 통해 텍스쳐를 샘플링, 샘플링 결과를 픽셀의 색상으로 사용하는 픽셀 쉐이더.
*/

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 normalWorld : COLOR;
    float2 texcoord : TEXCOORD;
};

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    return g_texture0.Sample(g_sampler, input.texcoord);
}