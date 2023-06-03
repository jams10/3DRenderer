#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

/*
*   상수 버퍼에 들어갈 데이터들을 묶어 개별 구조체들로 정의함.
*/
namespace NS
{
    using DirectX::SimpleMath::Matrix;
    using DirectX::SimpleMath::Vector3;

    // __declspec(align(256)) : 컴파일러에게 메모리 정렬을 256 바이트로 할 것임을 알려줌.
    __declspec(align(256)) struct MeshConstants  // 모델 마다 값이 다른 상수 버퍼.
    {
        Matrix world;
        Matrix worldInvTranspose;
    };

    __declspec(align(256)) struct GlobalConstants // 모델들이 공통으로 사용하는 상수 버퍼.
    {
        Matrix view;
        Matrix proj;
        Matrix viewProj;
        Matrix invProj;      // 역 projection 행렬. view 공간으로 바꿀 때 사용.
        Matrix invViewProj;  // 역 view-projection 행렬. world 공간으로 바꿀 때 사용.
        Vector3 eyeWorld;     // 시점(카메라) 월드 위치.
    };
}