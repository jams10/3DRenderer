#include "GraphicsProcessor.h"

#include <iostream>

#include "D3D11Graphics.h"

namespace NS
{
	using std::cout;

	GraphicsProcessor::GraphicsProcessor()
	{
		m_pD3D = nullptr;
	}

	GraphicsProcessor::~GraphicsProcessor()
	{
		if (m_pD3D != nullptr)
			Shutdown();
	}

	void GraphicsProcessor::Shutdown()
	{
		m_pD3D->ShutDownImGui();
		SAFE_RELEASE(m_pD3D)
	}

	void GraphicsProcessor::SetPipelineState(const GraphicsPSO& pso)
	{
		if (m_pD3D == nullptr)
		{
			cout << "GraphicsProcessor : Tried to set pipelinestate but, D3D11Graphics is null.\n";
			return;
		}

		m_pD3D->GetContext()->VSSetShader(pso.m_vertexShader.Get(), 0, 0);
		m_pD3D->GetContext()->PSSetShader(pso.m_pixelShader.Get(), 0, 0);
		m_pD3D->GetContext()->HSSetShader(pso.m_hullShader.Get(), 0, 0);
		m_pD3D->GetContext()->DSSetShader(pso.m_domainShader.Get(), 0, 0);
		m_pD3D->GetContext()->GSSetShader(pso.m_geometryShader.Get(), 0, 0);
		m_pD3D->GetContext()->IASetInputLayout(pso.m_inputLayout.Get());
		m_pD3D->GetContext()->RSSetState(pso.m_rasterizerState.Get());
		m_pD3D->GetContext()->OMSetDepthStencilState(pso.m_depthStencilState.Get(), 0);
		m_pD3D->GetContext()->IASetPrimitiveTopology(pso.m_primitiveTopology);
	}

	bool GraphicsProcessor::Initialize(int screenWidth, int screenHeight, HWND hWnd)
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
}