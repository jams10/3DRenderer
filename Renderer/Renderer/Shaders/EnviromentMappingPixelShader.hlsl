#include "Common.hlsli" // ���̴������� include ��� ����

TextureCube g_textureCube0 : register(t0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    // 1) ���� �븻 �������� ���ư��� ť��� �ؽ��ĸ� ���ø� �ϴ� ���.
    //return g_textureCube0.Sample(linearClampSampler, input.normalWorld.xyz);
    // 2) ���� ���Ͱ� ���� �븻 ���Ϳ� ���� ����� �ݻ� ���͸� ������ ť��� �ؽ��ĸ� ���ø� �ϴ� ���.
    // ������ ���� reflect �Լ��� ��� �������� ��ü�� ���ϴ� ���͸� �Է����� �־�� ��ü ǥ�鿡�� �ݻ�Ǿ� ������ ���͸� ���� �� ����.
    float3 eyeToVertex = normalize(input.posWorld - eyeWorld);
    return g_textureCube0.Sample(linearClampSampler, reflect(eyeToVertex, input.normalWorld.xyz));
}