#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

/*
*   ��� ���ۿ� �� �����͵��� ���� ���� ����ü��� ������.
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
    // __declspec(align(256))�� ����Ϸ��� ������, Scene Ŭ���� �ν��Ͻ� ���� �ÿ� �� �� ���� ���� �߻�.
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