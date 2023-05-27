#pragma once

#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <wrl.h>

#include "Utility/CustomMacros.h"

namespace NS
{
    using Microsoft::WRL::ComPtr;

	struct MeshForGPU
	{
        ComPtr<ID3D11VertexShader> pVertexShader;
        ComPtr<ID3D11PixelShader> pPixelShader;
        ComPtr<ID3D11InputLayout> pInputLayout;

        ComPtr<ID3D11Buffer> pVertexBuffer;
        ComPtr<ID3D11Buffer> pIndexBuffer;
        ComPtr<ID3D11Buffer> pConstantBuffer;

        ComPtr<ID3D11Texture2D> pDiffuseTexture;
        ComPtr<ID3D11ShaderResourceView> pDiffuseMapSRV;
	};
}