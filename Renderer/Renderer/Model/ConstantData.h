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
        Matrix model;
        Matrix view;
        Matrix projection;
    };

    static_assert((sizeof(ConstantDataMVP) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");

    struct ConstantDataMVPInvTranspose
    {
        Matrix model;
        Matrix invTranspose;
        Matrix view;
        Matrix projection;
    };

    static_assert((sizeof(ConstantDataMVPInvTranspose) % 16) == 0,
        "Constant Buffer size must be 16-byte aligned");
}