#pragma once

#include "Utility/CustomMacros.h"

#include <d3d11.h>
#include <wrl.h>

/*
*	<GraphicsPSO>
*	씬 렌더링을 위해 렌더링 파이프라인에 바인딩 해야 하는 쉐이더와 각 State들을 담고 있는 클래스.
*   특정 렌더링 작업에 필요한 쉐이더, state들을 미리 만들어 분류해 놓음으로써 렌더링 시 간단하게 PSO만 바꿔주면 되기 때문에
*   코드가 더 깔끔해지고 유지 보수 하기에 편함.
*/
namespace NS
{
    using Microsoft::WRL::ComPtr;

	class GraphicsPSO
	{
	public:
		void operator=(const GraphicsPSO& pso);

    public:
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11HullShader> m_hullShader;
        ComPtr<ID3D11DomainShader> m_domainShader;
        ComPtr<ID3D11GeometryShader> m_geometryShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;

        ComPtr<ID3D11BlendState> m_blendState;
        ComPtr<ID3D11DepthStencilState> m_depthStencilState;
        ComPtr<ID3D11RasterizerState> m_rasterizerState;

        D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	};
}



