
#include "Common.hlsli"

/*
*   �����Ǿ� ���� �ؽ��� ��ǥ�� ���� �ؽ��ĸ� ���ø�, ���ø� ����� �ȼ��� �������� ����ϴ� �ȼ� ���̴�.
*/

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    return g_texture0.Sample(g_sampler, input.texcoord);
}