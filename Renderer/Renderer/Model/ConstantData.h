#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

/*
*   상수 버퍼에 들어갈 데이터들을 묶어 개별 구조체들로 정의함.
*/
namespace NS
{
    using DirectX::SimpleMath::Matrix;

    struct ConstantDataMVP
    {
        Matrix world;
        Matrix view;
        Matrix projection;
    };
    // __declspec(align(256))을 사용하려고 했으나, Scene 클래스 인스턴스 해제 시에 알 수 없는 오류 발생.
    static_assert((sizeof(ConstantDataMVP) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");

    struct ConstantDataWorldInvTranspose
    {
        Matrix world;
        Matrix worldInvTranspose;
    };

    static_assert((sizeof(ConstantDataWorldInvTranspose) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");

    struct ConstantDataViewProj
    {
        Matrix view;
        Matrix proj;
    };

    static_assert((sizeof(ConstantDataViewProj) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");
}