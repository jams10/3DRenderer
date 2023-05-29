#pragma once

#include "Utility/CustomMacros.h"
#include "Windows/WindowsHeaders.h"

/*
*	<Graphics>
*	Direct3D�� ���� ������ ���������ο��� ����� �ڿ��� �����ϰ� �����ϴ� ������ ����ϴ� D3D11Graphics Ŭ������ �ܺθ� �����ϴ� ������ �����.
*	���߿� Direct3D�� �ƴ� �ٸ� �׷��Ƚ� ���̺귯���� ����� ��� �̰����� ���� Ŭ������ ������ �ܺο� ��������.
*/
namespace NS
{
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 100.0f;
	const float SCREEN_NEAR = 1.0f;

	class D3D11Graphics;

	class Graphics
	{
	public:
		Graphics();
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();

		bool Initialize(int screenWidth, int screenHeight, HWND hWnd);
		void Shutdown();

		inline D3D11Graphics* GetD3D11() { return m_pD3D; }

	private:
		D3D11Graphics* m_pD3D;
	};
}


