#pragma once

#include "Utility/CustomMacros.h"

#include <vector>

#include "GraphicsPSO.h"

/*
*	이 곳에서 원하는 렌더링 작업에 따라 각 PSO들을 미리 정의 해 놓음.
*   여기서는 외부에서 사용하기 위해 extern으로 정의부가 외부에 있음을 알리고 실제로 cpp 파일에서 정의함.
*/
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
		extern ComPtr<ID3D11VertexShader> vertexShader_PhongShading;
		extern ComPtr<ID3D11VertexShader> vertexShader_DrawingNormals;
		extern ComPtr<ID3D11PixelShader> pixelShader_Color;
		extern ComPtr<ID3D11PixelShader> pixelShader_TextureMapping;
		extern ComPtr<ID3D11PixelShader> pixelShader_PhongShading;

		// Input Layouts
		extern ComPtr<ID3D11InputLayout> inputLayout_Default;
		extern ComPtr<ID3D11InputLayout> inputLayout_DrawingNormals;

		// Pipeline States
		extern GraphicsPSO vertexColorPSO;
		extern GraphicsPSO vertexColorWirePSO;
		extern GraphicsPSO textureMappingPSO;
		extern GraphicsPSO phongShadingPSO;
		extern GraphicsPSO defaultWirePSO;
		extern GraphicsPSO drawingNormalPSO;

		void InitCommonStates(GraphicsProcessor* pGraphics);

		// 내부적으로 InitCommonStates()에서 사용
		void InitSamplers(GraphicsProcessor* pGraphics);
		void InitRasterizerStates(GraphicsProcessor* pGraphics);
		void InitDepthStencilStates(GraphicsProcessor* pGraphics);
		void InitShaders(GraphicsProcessor* pGraphics);
		void InitPipelineStates();
	}
}


