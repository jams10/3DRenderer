
#include "Common.hlsli"

/*
*   ��ġ, �븻, �ؽ��� ��ǥ�� ������ ������ Ʈ�������ϴ� ������ ���� ���̴�.
*/

cbuffer MeshConstants : register(b0)
{
    matrix world;
    matrix worldInvTranspose;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f); // ���� ����.
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);

    output.posProj = pos;
    output.normalWorld = input.normalModel;
    output.texcoord = input.texcoord;
    output.color = input.color;
    
    return output;
}