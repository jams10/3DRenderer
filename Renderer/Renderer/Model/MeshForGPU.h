#pragma once

#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <wrl.h>

#include "Utility/CustomMacros.h"

/*
*	GPU 쪽으로 넘겨줄 Mesh 데이터를 구조체로 정의함.
*	Mesh를 렌더링 하기 위해 필요한 쉐이더 및 각종 버퍼들을 담고 있음.
*/
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