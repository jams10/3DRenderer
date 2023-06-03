#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

// ���̴����� ���������� ����ϴ� ������ Ÿ���� ����.

// ���÷����� ��� ���̴����� �������� ���
SamplerState linearWrapSampler : register(s0);
SamplerState linearClampSampler : register(s1);

cbuffer GlobalConstants : register(b1)
{
    matrix view;
    matrix proj;
    matrix viewProj;
    matrix invProj;      // �� projection ���. view �������� �ٲ� �� ���.
    matrix invViewProj;  // �� view-projection ���. world �������� �ٲ� �� ���.
    float3 eyeWorld;     // ����(ī�޶�) ���� ��ġ.
}

struct VertexShaderInput
{
    float3 posModel : POSITION;   //�� ��ǥ���� ��ġ position
    float3 normalModel : NORMAL0; // �� ��ǥ���� normal    
    float2 texcoord : TEXCOORD0;
    float3 tangentModel : TANGENT0;
    float3 color : COLOR0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION;   // World position (���� ��꿡 ���)
    float3 normalWorld : NORMAL0;
    float2 texcoord : TEXCOORD0;
    float3 tangentWorld : TANGENT0;
    float3 color : COLOR0;
};

#endif // __COMMON_HLSLI__