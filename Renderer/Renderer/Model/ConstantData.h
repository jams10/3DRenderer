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
        BOOL bUseTexture = false;
        Vector3 specular = Vector3(0.5f);
        float dummy1;
        Vector3 color = Vector3(0.5f, 0.65f, 0.98f);
        float dummy2;
    };

    static_assert((sizeof(MaterialConstants) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");

    struct LightConstant
    {
        Vector3 intensity = Vector3(1.0f);
        float fallOffStart = 0.0f;
        Vector3 direction = Vector3(0.0f, 0.0f, 1.0f);
        float fallOffEnd = 10.0f;
        Vector3 position = Vector3(0.0f, 0.0f, -1.5f);
        float spotPower = 1.0f;
        Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
        int type = 0;
        Vector3 dummy = Vector3(0.f);
        BOOL turnOn = false;
    };

    static_assert((sizeof(LightConstant) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");

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

    // LightConstant�� ũ�Ⱑ ũ�Ƿ� __declspec(align(256)) �ٿ� ������ �迭 ���ʿ� �ִ� Light �����ʹ� ©�������� ������ ���̴� �ʿ��� ���� ����.
    struct GlobalSceneDataConstant // �𵨵��� �������� ����ϴ� �ȼ� ���̴� ��� ����.
    {
        LightConstant lights[MAX_LIGHTS];
    };
}