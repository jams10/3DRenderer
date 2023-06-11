#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

#define MAX_LIGHTS 5

/*
*   상수 버퍼에 들어갈 데이터들을 묶어 개별 구조체들로 정의함.
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

    // __declspec(align(256)) : 컴파일러에게 메모리 정렬을 256 바이트로 할 것임을 알려줌.
    __declspec(align(256)) struct MeshConstants  // 모델 마다 값이 다른 상수 버퍼.
    {
        Matrix world;
        Matrix worldInvTranspose;
    };

    __declspec(align(256)) struct GlobalCameraTransformConstant // 모델들이 공통으로 사용하는 정점 쉐이더 상수 버퍼.
    {
        Matrix view;
        Matrix proj;
        Matrix viewProj;
        Matrix invProj;      // 역 projection 행렬. view 공간으로 바꿀 때 사용.
        Matrix invViewProj;  // 역 view-projection 행렬. world 공간으로 바꿀 때 사용.
        Vector3 eyeWorld;    // 시점(카메라) 월드 위치.
    };

    // LightConstant가 크기가 크므로 __declspec(align(256)) 붙여 버리면 배열 뒤쪽에 있는 Light 데이터는 짤려버리기 때문에 쉐이더 쪽에서 받지 못함.
    struct GlobalSceneDataConstant // 모델들이 공통으로 사용하는 픽셀 쉐이더 상수 버퍼.
    {
        LightConstant lights[MAX_LIGHTS];
    };
}