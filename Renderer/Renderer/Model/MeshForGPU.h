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
        ComPtr<ID3D11Buffer> vertexBuffer;
        ComPtr<ID3D11Buffer> indexBuffer;
        ComPtr<ID3D11Buffer> vertexConstantBuffer;
        ComPtr<ID3D11Buffer> pixelConstantBuffer;

        ComPtr<ID3D11Texture2D> albedoTexture;
        ComPtr<ID3D11ShaderResourceView> albedoSRV;

        UINT indexCount = 0; // Number of indiecs = 3 * number of triangles
        UINT vertexCount = 0;
        UINT stride = 0;
        UINT offset = 0;

        void Shutdown()
        {
            vertexBuffer.Reset();
            indexBuffer.Reset();
            vertexConstantBuffer.Reset();
            pixelConstantBuffer.Reset();

            albedoSRV.Reset();
            albedoTexture.Reset();
        }
	};
}