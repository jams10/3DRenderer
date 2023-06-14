
#include "Common.hlsli"

/*
*   �븻 ���͸� �׸��� ���� ����ϴ� ���� ���̴�.
*/

cbuffer MeshConstants : register(b0)
{
    matrix world;
    matrix worldInvTranspose;
};

cbuffer DrawingNormalConstants : register(b1)
{
    float scale;
    float3 dummy;
}

PixelShaderInput main(NormalVertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f);

    // Normal ���� ��ȯ.
    float3 normal = mul(float4(input.normalModel, 0.0f), worldInvTranspose).xyz;
    normal = normalize(normal);
    // ���� ��ġ ��ȯ.
    pos = mul(pos, world);
    float t = input.texcoord.x; // 0�� ��� �븻 ���� ������. 1�� ��� �븻 ���� ������ �ǹ�.
    pos.xyz += normal * t * scale; // �븻 ���� ������, ���� ���ο� ���� ������ �븻 ���� �������� �̵� ������.
    
    pos = mul(pos, view);
    pos = mul(pos, proj);
    output.posProj = pos;
    
    output.color = float3(1.0, 1.0, 0.0) * (1.0 - t) + float3(1.0, 0.0, 0.0) * t;
    
    return output;
}

