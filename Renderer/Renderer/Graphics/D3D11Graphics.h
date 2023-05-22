#pragma once

#include "Utility/CustomMacros.h"
#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <exception>
#include <wrl.h>

/*
 *	Direct3D11 ���̺귯���� �̿�, �׷��Ƚ� ���� �۾��� �����ϴ� Ŭ����. Graphics Ŭ������ ���ؼ��� ����.
 */

namespace NS
{
	using Microsoft::WRL::ComPtr;

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
		friend class Graphics;

	public:
		D3D11Graphics();
		D3D11Graphics(const D3D11Graphics&) = delete;
		D3D11Graphics& operator=(const D3D11Graphics&) = delete;
		~D3D11Graphics();

		bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hWnd, float screenDepth, float screenNear);

		void BeginFrame(float red, float green, float blue, float alpha);
		void EndFrame();

		bool CreateRenderTargetView();
		void SetViewport(int screenWidth, int screenHeight);
		bool CreateDepthBuffer(int screenWidth, int screenHeight);

	private:
		// �⺻ ������ �ʿ� �ڿ�.
		ComPtr<ID3D11Device> m_pDevice;
		ComPtr<ID3D11DeviceContext> m_pContext;
		ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV;
		ComPtr<IDXGISwapChain> m_pSwapChain;
		ComPtr<ID3D11RasterizerState> m_pSolidRasterizerSate;

		// Depth buffer ����
		ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;

		D3D11_VIEWPORT m_screenViewport;
		UINT m_numQualityLevels;
		int m_screenWidth;
		int m_screenHeight;
	};
}



