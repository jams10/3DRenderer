#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

/*
*   ��� ���ۿ� �� �����͵��� ���� ���� ����ü��� ������.
*/
namespace NS
{
    using DirectX::SimpleMath::Matrix;
    using DirectX::SimpleMath::Vector3;

    // __declspec(align(256)) : �����Ϸ����� �޸� ������ 256 ����Ʈ�� �� ������ �˷���.
    __declspec(align(256)) struct MeshConstants  // �� ���� ���� �ٸ� ��� ����.
    {
        Matrix world;
        Matrix worldInvTranspose;
    };

    __declspec(align(256)) struct GlobalConstants // �𵨵��� �������� ����ϴ� ��� ����.
    {
        Matrix view;
        Matrix proj;
        Matrix viewProj;
        Matrix invProj;      // �� projection ���. view �������� �ٲ� �� ���.
        Matrix invViewProj;  // �� view-projection ���. world �������� �ٲ� �� ���.
        Vector3 eyeWorld;     // ����(ī�޶�) ���� ��ġ.
    };
}