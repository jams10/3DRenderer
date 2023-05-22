#pragma once

#include "Utility/CustomMacros.h"
#include "Windows/WindowsHeaders.h"

#include <d3d11.h>
#include <exception>
#include <wrl.h>

/*
 *	Direct3D11 라이브러리를 이용, 그래픽스 관련 작업을 수행하는 클래스. Graphics 클래스를 통해서만 접근.
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
		// 기본 렌더링 필요 자원.
		ComPtr<ID3D11Device> m_pDevice;
		ComPtr<ID3D11DeviceContext> m_pContext;
		ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV;
		ComPtr<IDXGISwapChain> m_pSwapChain;
		ComPtr<ID3D11RasterizerState> m_pSolidRasterizerSate;

		// Depth buffer 관련
		ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;

		D3D11_VIEWPORT m_screenViewport;
		UINT m_numQualityLevels;
		int m_screenWidth;
		int m_screenHeight;
	};
}



