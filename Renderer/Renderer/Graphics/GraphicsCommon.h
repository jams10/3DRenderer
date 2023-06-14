#pragma once

#include "Utility/CustomMacros.h"

#include <vector>

#include "GraphicsPSO.h"

/*
*	�� ������ ���ϴ� ������ �۾��� ���� �� PSO���� �̸� ���� �� ����.
*   ���⼭�� �ܺο��� ����ϱ� ���� extern���� ���Ǻΰ� �ܺο� ������ �˸��� ������ cpp ���Ͽ��� ������.
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
		extern ComPtr<ID3D11RasterizerState> rasterizerState_SolidCW; // CW = �ð� ���� winding.
		extern ComPtr<ID3D11RasterizerState> rasterizerState_WireCW;  // ���̾� ������ ��� �����Ͷ�����.

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

		// ���������� InitCommonStates()���� ���
		void InitSamplers(GraphicsProcessor* pGraphics);
		void InitRasterizerStates(GraphicsProcessor* pGraphics);
		void InitDepthStencilStates(GraphicsProcessor* pGraphics);
		void InitShaders(GraphicsProcessor* pGraphics);
		void InitPipelineStates();
	}
}


