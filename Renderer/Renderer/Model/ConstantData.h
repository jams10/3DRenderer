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
}