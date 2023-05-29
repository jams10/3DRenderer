
/*
*   ��ġ, �븻, �ؽ��� ��ǥ�� ������ ������ Ʈ�������ϴ� ������ ���� ���̴�.
*/

cbuffer MVPMatrix : register(b0)
{
    matrix model;
    matrix invTranspose; // normal ���� Ʈ������ ��ȯ �뵵.
    matrix view;
    matrix projection;
};

struct VertexShaderInput
{
    float3 posModel : POSITION;
    float3 normalModel : NORMAL;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 normalWorld : COLOR;
    float2 texcoord : TEXCOORD;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f); // ���� ����.
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.normalWorld = input.normalModel;
    output.texcoord = input.texcoord;
    
    return output;
}