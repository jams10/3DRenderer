#pragma once

#include "Utility/CustomMacros.h"
#include "Windows/WindowsHeaders.h"
#include "GraphicsPSO.h"

#include "Model/ConstantData.h"

/*
*	<Graphics>
*	Direct3D를 통해 렌더링 파이프라인에서 사용할 자원을 생성하고 세팅하는 역할을 담당하는 D3D11Graphics 클래스와 외부를 연결하는 역할을 담당함.
*	나중에 Direct3D가 아닌 다른 그래픽스 라이브러리를 사용할 경우 이곳에서 관련 클래스를 생성해 외부와 연결해줌.
*/
namespace NS
{
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 100.0f;
	const float SCREEN_NEAR = 1.0f;

	class D3D11Graphics;

	class GraphicsProcessor
	{
	public:
		GraphicsProcessor();
		GraphicsProcessor(const GraphicsProcessor&) = delete;
		GraphicsProcessor& operator=(const GraphicsProcessor&) = delete;
		~GraphicsProcessor();

		bool Initialize(int screenWidth, int screenHeight, HWND hWnd);
		void Shutdown();

		void SetPipelineState(const GraphicsPSO& pso);

		inline D3D11Graphics* GetD3D11() { return m_pD3D; }

	private:
		D3D11Graphics* m_pD3D;
	};
}


