#include "Common.hlsli" // ���̴������� include ��� ����

TextureCube g_textureCube0 : register(t0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    // ť����� ������ ��ġ�ϰ� ��ü ���� ���δ� Ŀ�ٶ� ������ü��.
    // ���� ť��� �ؽ��ĸ� ���ø� �� ���� ���� ��ǥ ������ �߽����� ���� ��ü�� �����ϴ� �� ������ ��ġ�� �������� ���ø� ���ָ� ��.
    // ���� ��ǥ�� ���ø� �ؽ��� ��ǥ�̹Ƿ� float3 Ÿ��.
    return g_textureCube0.Sample(linearClampSampler, input.posWorld.xyz);
}