
#include "Common.hlsli"

/*
*   ������ �����Ǿ� ���� ������ ������ ����ϴ� �ȼ� ���̴�.
*/

float4 main(PixelShaderInput input) : SV_TARGET
{
    // ������ �����Ǿ� ������ ������ ��ġ�� ���������� �� ���� �����Ǿ� ����.
    return float4(input.color, 1.0);
}