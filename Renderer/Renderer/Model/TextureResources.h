#pragma once

#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <wrl.h>
#include <string>

#include "Utility/CustomMacros.h"

/*
*	�𵨿� ����� �ؽ��� �ڿ����� ����.
*/
namespace NS
{
    using Microsoft::WRL::ComPtr;

    struct TextureResourcesForCPU
    {
        std::string albedoTextureFilename;
        std::string enviromentMapTexture;
    };

    struct TextureResourcesForGPU
    {
        ComPtr<ID3D11Texture2D> albedoTexture;
        ComPtr<ID3D11ShaderResourceView> albedoSRV;
        ComPtr<ID3D11Texture2D> enviromentMapTexture;
        ComPtr<ID3D11ShaderResourceView> enviromentMapSRV;

        void Shutdown()
        {
            albedoSRV.Reset();
            albedoTexture.Reset();
            enviromentMapTexture.Reset();
            enviromentMapSRV.Reset();
        }
    };
}