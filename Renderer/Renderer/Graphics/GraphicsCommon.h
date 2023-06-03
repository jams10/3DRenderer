#pragma once

#include "Utility/CustomMacros.h"

#include <vector>

#include "GraphicsPSO.h"

namespace NS
{
	class GraphicsProcessor;
	
	namespace Graphics
	{
		// Samplers
		extern ComPtr<ID3D11SamplerState> samplerState_LinearWrap;
		extern ComPtr<ID3D11SamplerState> samplerState_LinearClamp;
		extern std::vector<ID3D11SamplerState*> samplerStates;

		// Rasterizer States
		extern ComPtr<ID3D11RasterizerState> rasterizerState_SolidCW; // CW = 시계 방향 winding.
		extern ComPtr<ID3D11RasterizerState> rasterizerState_WireCW;  // 와이어 프레임 모드 래스터라이저.

		// Depth Stencil States
		extern ComPtr<ID3D11DepthStencilState> depthStencilState_Default;

		// Shaders
		extern ComPtr<ID3D11VertexShader> vertexShader_Color;
		extern ComPtr<ID3D11VertexShader> vertexShader_TextureMapping;
		extern ComPtr<ID3D11PixelShader> pixelShader_Color;
		extern ComPtr<ID3D11PixelShader> pixelShader_TextureMapping;

		// Input Layouts
		extern ComPtr<ID3D11InputLayout> inputLayout_Default;

		// Pipeline States
		extern GraphicsPSO vertexColorPSO;
		extern GraphicsPSO textureMappingPSO;
		extern GraphicsPSO defaultWirePSO;

		void InitCommonStates(GraphicsProcessor* pGraphics);

		// 내부적으로 InitCommonStates()에서 사용
		void InitSamplers(GraphicsProcessor* pGraphics);
		void InitRasterizerStates(GraphicsProcessor* pGraphics);
		void InitDepthStencilStates(GraphicsProcessor* pGraphics);
		void InitShaders(GraphicsProcessor* pGraphics);
		void InitPipelineStates();
	}
}


