#include "Graphics.h"

#include <iostream>

#include "D3D11Graphics.h"

namespace NS
{
	using std::cout;

	Graphics::Graphics()
	{
		m_pD3D = nullptr;
	}

	Graphics::~Graphics()
	{

	}

	bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hWnd)
	{
		m_pD3D = new D3D11Graphics();
		if (m_pD3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, SCREEN_DEPTH, SCREEN_NEAR) == false)
		{
			cout << "Failed : D3D Initialize.\n";
			__ERRORLINE__
			return false;
		}

		return true;
	}

	bool Graphics::BeginFrame(float red, float green, float blue, float alpha)
	{
		m_pD3D->BeginFrame(red, green, blue, alpha);
		return true;
	}

	bool Graphics::EndFrame()
	{
		m_pD3D->EndFrame();
		return true;
	}
}