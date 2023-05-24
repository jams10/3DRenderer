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
        ComPtr<ID3D11VertexShader> m_pVertexShader;
        ComPtr<ID3D11PixelShader> m_pPixelShader;
        ComPtr<ID3D11InputLayout> m_pInputLayout;

        ComPtr<ID3D11Buffer> m_pVertexBuffer;
        ComPtr<ID3D11Buffer> m_pIndexBuffer;
        ComPtr<ID3D11Buffer> m_pConstantBuffer;
	};
}