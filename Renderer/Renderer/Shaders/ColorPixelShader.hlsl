
/*
*   ������ �����Ǿ� ���� ������ ������ ����ϴ� �ȼ� ���̴�.
*/

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    // ������ �����Ǿ� ������ ������ ��ġ�� ���������� �� ���� �����Ǿ� ����.
    return float4(input.color, 1.0);
}