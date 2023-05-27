#pragma once

#include "Utility/CustomMacros.h"

#include <directxtk/SimpleMath.h>

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