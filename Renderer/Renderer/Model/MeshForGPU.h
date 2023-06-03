#pragma once

#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <wrl.h>

#include "Utility/CustomMacros.h"

/*
*	GPU ������ �Ѱ��� Mesh �����͸� ����ü�� ������.
*	Mesh�� ������ �ϱ� ���� �ʿ��� ���̴� �� ���� ���۵��� ��� ����.
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