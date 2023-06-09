#include "GraphicsCommon.h"

#include "GraphicsProcessor.h"
#include "D3D11Graphics.h"

namespace NS
{
	namespace Graphics
	{
		// Samplers
		ComPtr<ID3D11SamplerState> samplerState_LinearWrap;
		ComPtr<ID3D11SamplerState> samplerState_LinearClamp;
		std::vector<ID3D11SamplerState*> samplerStates;

		// Rasterizer States
		ComPtr<ID3D11RasterizerState> rasterizerState_SolidCW;
		ComPtr<ID3D11RasterizerState> rasterizerState_WireCW;

		// Depth Stencil States
		ComPtr<ID3D11DepthStencilState> depthStencilState_Default;

		// Shaders
		ComPtr<ID3D11VertexShader> vertexShader_Color;
		ComPtr<ID3D11VertexShader> vertexShader_TextureMapping;
		ComPtr<ID3D11VertexShader> vertexShader_PhongShading;
		ComPtr<ID3D11PixelShader> pixelShader_Color;
		ComPtr<ID3D11PixelShader> pixelShader_TextureMapping;
		ComPtr<ID3D11PixelShader> pixelShader_PhongShading;

		// Input Layouts
		ComPtr<ID3D11InputLayout> inputLayout_Default;

		// Pipeline States
		GraphicsPSO vertexColorPSO;
		GraphicsPSO textureMappingPSO;
		GraphicsPSO phongShadingPSO;
		GraphicsPSO defaultWirePSO;

		void Graphics::InitCommonStates(GraphicsProcessor* pGraphics)
		{
			InitShaders(pGraphics);
			InitSamplers(pGraphics);
			InitRasterizerStates(pGraphics);
			InitDepthStencilStates(pGraphics);
			InitPipelineStates();
		}

		void Graphics::InitSamplers(GraphicsProcessor* pGraphics)
		{
			ComPtr<ID3D11Device> device = pGraphics->GetD3D11()->GetDevice();

			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			device->CreateSamplerState(&sampDesc, samplerState_LinearWrap.GetAddressOf());
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			device->CreateSamplerState(&sampDesc, samplerState_LinearClamp.GetAddressOf());

			// 샘플러 순서가 "Common.hlsli"에서와 일관성 있어야 함
			samplerStates.push_back(samplerState_LinearWrap.Get());
			samplerStates.push_back(samplerState_LinearClamp.Get());
		}

		void Graphics::InitRasterizerStates(GraphicsProcessor* pGraphics)
		{
			ComPtr<ID3D11Device> device = pGraphics->GetD3D11()->GetDevice();

			// Rasterizer States
			D3D11_RASTERIZER_DESC rastDesc;
			ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
			rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
			rastDesc.FrontCounterClockwise = false;
			rastDesc.DepthClipEnable = true;
			rastDesc.MultisampleEnable = true;
			
			THROWFAILED(device->CreateRasterizerState(&rastDesc, rasterizerState_SolidCW.GetAddressOf()));
			rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
			THROWFAILED(device->CreateRasterizerState(&rastDesc, rasterizerState_WireCW.GetAddressOf()));
		}

		void Graphics::InitDepthStencilStates(GraphicsProcessor* pGraphics)
		{
			ComPtr<ID3D11Device> device = pGraphics->GetD3D11()->GetDevice();

			// Depth Stencil State(깊이 값을 어떻게 비교할 것인가를 결정)를 생성.
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
			depthStencilDesc.DepthEnable = true; // false
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
			THROWFAILED(device->CreateDepthStencilState(&depthStencilDesc, depthStencilState_Default.GetAddressOf()));
		}

		void Graphics::InitShaders(GraphicsProcessor* pGraphics)
		{
			vector<D3D11_INPUT_ELEMENT_DESC> basicIEs = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				 D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
				 D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12 + 4 * 3,
				 D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24 + 4 * 2,
				 D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32 + 4 * 3,
				 D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\ColorVertexShader.hlsl", 
				basicIEs, vertexShader_Color, inputLayout_Default);
			pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\TextureMappingVertexShader.hlsl", 
				basicIEs, vertexShader_TextureMapping, inputLayout_Default);
			pGraphics->GetD3D11()->CreateVertexShaderAndInputLayout(L"Shaders\\PhongVertexShader.hlsl",
				basicIEs, vertexShader_PhongShading, inputLayout_Default);

			pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\ColorPixelShader.hlsl", pixelShader_Color);
			pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\TextureMappingPixelShader.hlsl", pixelShader_TextureMapping);
			pGraphics->GetD3D11()->CreatePixelShader(L"Shaders\\PhongPixelShader.hlsl", pixelShader_PhongShading);
		}

		void InitPipelineStates()
		{
			vertexColorPSO.m_vertexShader = vertexShader_Color;
			vertexColorPSO.m_pixelShader = pixelShader_Color;
			vertexColorPSO.m_inputLayout = inputLayout_Default;
			vertexColorPSO.m_depthStencilState = depthStencilState_Default;
			vertexColorPSO.m_rasterizerState = rasterizerState_SolidCW;
			vertexColorPSO.m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

			textureMappingPSO = vertexColorPSO;
			textureMappingPSO.m_vertexShader = vertexShader_TextureMapping;
			textureMappingPSO.m_pixelShader = pixelShader_TextureMapping;

			phongShadingPSO = textureMappingPSO;
			phongShadingPSO.m_vertexShader = vertexShader_PhongShading;
			phongShadingPSO.m_pixelShader = pixelShader_PhongShading;

			defaultWirePSO = textureMappingPSO;
			defaultWirePSO.m_rasterizerState = rasterizerState_WireCW;
		}
	}
}