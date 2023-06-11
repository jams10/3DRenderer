#pragma once

#include "Utility/CustomMacros.h"
#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <exception>
#include <vector>
#include <string>
#include <wrl.h>

#include "Model/MeshForCPU.h"
#include "Model/MeshForGPU.h"
#include "Model/ConstantData.h"

 /*
 *	<D3D11Graphics>
 *	Direct3D11 라이브러리를 이용, 그래픽스 관련 작업을 수행하는 클래스. Graphics 클래스를 통해서만 접근.
 */
namespace NS
{
	using Microsoft::WRL::ComPtr;
	using std::vector;
	using std::wstring;

#define THROWFAILED(x) ThrowIfFailed(x)

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}

	class D3D11Graphics
	{
		friend class GraphicsProcessor;

	public:
		D3D11Graphics();
		D3D11Graphics(const D3D11Graphics&) = delete;
		D3D11Graphics& operator=(const D3D11Graphics&) = delete;
		~D3D11Graphics();

		bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hWnd, float screenDepth, float screenNear);

		void BeginFrame(float red, float green, float blue, float alpha);
		void Render(const MeshForGPU& meshForGPU);
		void EndFrame();

		void CreateGlobalCameraTransformConstantBuffer(const GlobalCameraTransformConstant& globalCameraTransformConstant);
		void CreateGlobalSceneDataConstantBuffer(const GlobalSceneDataConstant& globalSceneDataConstant);
		void SetGlobalConstantBuffers();
		void UpdateGlobalCameraTransformConstantBuffer(const GlobalCameraTransformConstant& globalCameraTransformConstant);
		void UpdateGlobalSceneDataConstantBuffer(const GlobalSceneDataConstant& globalSceneDataConstant);

		float GetAspectRatio();
		void ResizeScreen(int screenWidth, int screenHeight);
		void ShutDownImGui();

		ComPtr<ID3D11Device> GetDevice() { return m_device; }
		ComPtr<ID3D11DeviceContext> GetContext() { return m_context; }

	private:
		// 기본 렌더링 필요 자원.
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
		ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
		ComPtr<IDXGISwapChain> m_swapChain;

		// Depth buffer 관련
		ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;

		// 전역 상수 버퍼.
		ComPtr<ID3D11Buffer> m_globalCameraTransformConstantBuffer = nullptr;
		ComPtr<ID3D11Buffer> m_globalSceneDataConstantBuffer = nullptr;

		D3D11_VIEWPORT m_screenViewport;
		UINT m_numQualityLevels;
		int m_screenWidth;
		int m_screenHeight;

	public:
#pragma region Pipeline Functions
		void CreateVertexShaderAndInputLayout(
			const wstring& filename,
			const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
			ComPtr<ID3D11VertexShader>& vertexShader,
			ComPtr<ID3D11InputLayout>& inputLayout);

		void CreatePixelShader(const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader);

		void CreateIndexBuffer(const vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& m_indexBuffer);

		template <typename T_VERTEX>
		void CreateVertexBuffer(const vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer) 
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 정점 버퍼의 경우 한번 GPU로 보내면 바꿀 필요가 없음. 트랜스폼은 변환 행렬을 통해 변경 가능.
			bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0; // CPU 접근 필요 없음.
			bufferDesc.StructureByteStride = sizeof(T_VERTEX);

			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // 정점 버퍼에 들어갈 실제 데이터를 지정함.
			vertexBufferData.pSysMem = vertices.data();
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;

			THROWFAILED(m_device->CreateBuffer(&bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf()));
		}

		template <typename T_CONSTANT>
		void CreateConstantBuffer(const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer) 
		{
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(constantBufferData);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 상수 버퍼의 경우 CPU 쪽에서 map을 통해 값을 업데이트 해야함.
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &constantBufferData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			THROWFAILED(m_device->CreateBuffer(&cbDesc, &InitData, constantBuffer.GetAddressOf()));
		}

		template <typename T_DATA>
		void UpdateBuffer(const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer) 
		{
			D3D11_MAPPED_SUBRESOURCE ms;
			m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			memcpy(ms.pData, &bufferData, sizeof(bufferData));
			m_context->Unmap(buffer.Get(), NULL);
		}

		bool CreateRenderTargetView();
		void SetViewport(int screenWidth, int screenHeight);
		bool CreateDepthBuffer(int screenWidth, int screenHeight);
		void CreateTexture(const std::string filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
#pragma endregion Pipeline Functions
	};
}



