#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

#define MAX_LIGHTS 5

/*
*   ��� ���ۿ� �� �����͵��� ���� ���� ����ü��� ������.
*/
namespace NS
{
    using DirectX::SimpleMath::Matrix;
    using DirectX::SimpleMath::Vector3;

    struct MaterialConstants
    {
        Vector3 ambient = Vector3(0.1f);
        float shininess = 1.0f;
        Vector3 diffuse = Vector3(0.5f);
        int bUseTexture = 0u;
        Vector3 specular = Vector3(0.5f);
        float dummy2;
    };

    struct LightConstant
    {
        Vector3 intensity = Vector3(1.0f);
        float fallOffStart = 0.0f;
        Vector3 direction = Vector3(0.0f, 0.0f, 1.0f);
        float fallOffEnd = 10.0f;
        Vector3 position = Vector3(0.0f, 2.0f, 0.0f);
        float spotPower = 1.0f;
        Vector3 color = Vector3(0.0f, 0.0f, 0.0f);
        int type = 0;
    };

    // __declspec(align(256)) : �����Ϸ����� �޸� ������ 256 ����Ʈ�� �� ������ �˷���.
    __declspec(align(256)) struct MeshConstants  // �� ���� ���� �ٸ� ��� ����.
    {
        Matrix world;
        Matrix worldInvTranspose;
    };

    __declspec(align(256)) struct GlobalCameraTransformConstant // �𵨵��� �������� ����ϴ� ���� ���̴� ��� ����.
    {
        Matrix view;
        Matrix proj;
        Matrix viewProj;
        Matrix invProj;      // �� projection ���. view �������� �ٲ� �� ���.
        Matrix invViewProj;  // �� view-projection ���. world �������� �ٲ� �� ���.
        Vector3 eyeWorld;    // ����(ī�޶�) ���� ��ġ.
    };

    __declspec(align(256)) struct GlobalSceneDataConstant // �𵨵��� �������� ����ϴ� �ȼ� ���̴� ��� ����.
    {
        LightConstant lights[MAX_LIGHTS];
    };
}