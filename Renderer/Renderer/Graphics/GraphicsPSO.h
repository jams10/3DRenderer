#pragma once

#include "Utility/CustomMacros.h"

#include <d3d11.h>
#include <wrl.h>

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



