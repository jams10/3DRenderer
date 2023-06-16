#include "D3D11Graphics.h"

#include <d3dcompiler.h>
#include <iostream>
#include <string>
#include <imgui_impl_dx11.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma comment(lib,"d3d11.lib")        // Direct3D 함수들이 정의된 라이브러리를 링크해줌.
#pragma comment(lib, "D3DCompiler.lib") // 셰이더를 런타임에 컴파일 해줄 때 사용할 수 있지만, 우리는 셰이더를 불러오는 함수를 사용하기 위해 연결해줬음. 

namespace NS
{
	using std::cout;

	D3D11Graphics::D3D11Graphics()
	{
		m_screenViewport = D3D11_VIEWPORT{};
		m_numQualityLevels = 0;
		m_screenWidth = 0;
		m_screenHeight = 0;
	}

	D3D11Graphics::~D3D11Graphics()
	{
		cout << "Destroy D3DGraphics." << '\n';
	}

	void D3D11Graphics::ShutDownImGui()
	{
		cout << "Shutdown Imgui DX11." << '\n';
		ImGui_ImplDX11_Shutdown();
	}

	bool D3D11Graphics::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hWnd, float screenDepth, float screenNear)
	{
		const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // 디버그 모드에서 디버깅 플래그 켜주기.
#endif

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		const D3D_FEATURE_LEVEL featureLevels[2] = {
			D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
			D3D_FEATURE_LEVEL_9_3 };
		D3D_FEATURE_LEVEL featureLevel;

#pragma region Create Device & Context
		THROWFAILED(D3D11CreateDevice(
			nullptr,					// Specify nullptr to use the default adapter.
			driverType,					// Create a device using the hardware graphics driver.
			0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
			createDeviceFlags,			// Set debug and Direct2D compatibility flags.
			featureLevels,				// List of feature levels this app can support.
			ARRAYSIZE(featureLevels),	// Size of the list above.
			D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
			&device,					// Returns the Direct3D device created.
			&featureLevel,				// Returns feature level of device created.
			&context					// Returns the device immediate context.
		));

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			cout << "D3D Feature Level 11 unsupported.\n";
			return false;
		}

		// 생성한 device, device context COM 인터페이스를 가져와 캐싱.
		THROWFAILED(device.As(&m_device));

		THROWFAILED(context.As(&m_context));
#pragma endregion Create Device & Context

		// 4X MSAA 지원하는지 확인
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_numQualityLevels);
		if (m_numQualityLevels <= 0)
		{
			cout << "MSAA not supported.\n";
		}

#pragma region Create Swapchain & Backbuffer
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = m_screenWidth;               // set the back buffer width
		sd.BufferDesc.Height = m_screenHeight;             // set the back buffer height
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
		sd.BufferCount = 2;                                // Double-buffering
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
		sd.OutputWindow = hWnd;                            // the window to be used
		sd.Windowed = TRUE;                                // windowed/full-screen mode
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		if (m_numQualityLevels > 0)  // MSAA 사용 가능하면 샘플 수를 늘려줌.(anti-aliasing)
		{
			sd.SampleDesc.Count = 4; // how many multisamples
			sd.SampleDesc.Quality = m_numQualityLevels - 1;
		}
		else 
		{
			sd.SampleDesc.Count = 1; // how many multisamples
			sd.SampleDesc.Quality = 0;
		}

		THROWFAILED(D3D11CreateDeviceAndSwapChain(0,
					driverType,
					0, // No software device
					createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_swapChain, &m_device, &featureLevel,
					&m_context));
#pragma endregion Create Swapchain & Backbuffer

		if (CreateRenderTargetView() == false) return false;

		SetViewport(m_screenWidth, m_screenHeight);

#pragma region Depth Buffer
		CreateDepthBuffer(m_screenWidth, m_screenHeight);

#pragma endregion Depth Buffer

		// imgui dx11 구현 초기화.
		if (ImGui_ImplDX11_Init(m_device.Get(), m_context.Get()) == false)
		{
			cout << "Failed : ImGui_ImplDX11_Init()\n";
			__ERRORLINE__
				return false;
		}

		cout << "Success : D3D has initialized!\n";
		return true;
	}

	void D3D11Graphics::BeginFrame(float red, float green, float blue, float alpha)
	{
		SetViewport(m_screenWidth, m_screenHeight); // 그려줄 영역인 뷰포트 설정.

		// RTV와 DSV 초기화. 화면을 지정 색상으로 날려주고, 깊이 버퍼도 초기화 해줌.
		float clearColor[4] = { red, green, blue, alpha };
		m_context->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);
		m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* targets[] = { m_backBufferRTV.Get() };
		m_context->OMSetRenderTargets(1, targets, m_depthStencilView.Get());

		// 이제 파이프라인 상태 설정은 GraphicsPSO, GraphicsProcessor 클래스의 SetPipelineState를 통해 설정함.
	}

	void D3D11Graphics::Render(const MeshForGPU& meshForGPU)
	{
		// 정점 쉐이더 상수 버퍼(모델별로 가지고 있을) 파이프라인에 바인딩.
		m_context->VSSetConstantBuffers(0, 1, meshForGPU.vertexConstantBuffer.GetAddressOf());

		// 픽셀 쉐이더 상수 버퍼 파이프라인에 바인딩.
		m_context->PSSetConstantBuffers(0, 1, meshForGPU.pixelConstantBuffer.GetAddressOf());

		// 사용할 쉐이더 리소스(텍스쳐) 바인딩.
		// TODO : 여러 텍스쳐 맵 SRV 추가.
		ID3D11ShaderResourceView* pixelResources[1] = { meshForGPU.albedoSRV.Get() };
		m_context->PSSetShaderResources(0, 1, pixelResources);

		// 정점, 인덱스 버퍼 설정하고 그리기 명령 호출.s
		m_context->IASetVertexBuffers(0, 1, meshForGPU.vertexBuffer.GetAddressOf(), &meshForGPU.stride, &meshForGPU.offset);
		m_context->IASetIndexBuffer(meshForGPU.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_context->DrawIndexed(meshForGPU.indexCount, 0, 0);
	}

	void D3D11Graphics::Render(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, vector<ID3D11Buffer*> constBufferVS, vector<ID3D11Buffer*> constBufferPS, 
		vector<ID3D11ShaderResourceView*> shaderResources, const UINT& indexCount, const UINT& stride, const UINT& offset)
	{
		// 정점 쉐이더 상수 버퍼(모델별로 가지고 있을) 파이프라인에 바인딩.
		if(!constBufferVS.empty())
			m_context->VSSetConstantBuffers(0, (UINT)constBufferVS.size(), constBufferVS.data());

		// 픽셀 쉐이더 상수 버퍼 파이프라인에 바인딩.
		if (!constBufferPS.empty())
			m_context->PSSetConstantBuffers(0, (UINT)constBufferPS.size(), constBufferPS.data());

		// 사용할 쉐이더 리소스(텍스쳐) 바인딩.
		// TODO : 여러 텍스쳐 맵 SRV 추가.
		if (!shaderResources.empty())
			m_context->PSSetShaderResources(0, (UINT)shaderResources.size(), shaderResources.data());

		// 정점, 인덱스 버퍼 설정하고 그리기 명령 호출.s
		m_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		m_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_context->DrawIndexed(indexCount, 0, 0);
	}

	void D3D11Graphics::EndFrame()
	{
		// Backbuffer에 그려진 화면을 모니터에 출력.
		m_swapChain->Present(1, 0);
	}

	void D3D11Graphics::CreateGlobalCameraTransformConstantBuffer(const GlobalCameraTransformConstant& globalCameraTransformConstant)
	{
		if (m_globalCameraTransformConstantBuffer == nullptr)
		{
			CreateConstantBuffer(globalCameraTransformConstant, m_globalCameraTransformConstantBuffer);
		}
		else
		{
			m_globalCameraTransformConstantBuffer.Get()->Release();
			CreateConstantBuffer(globalCameraTransformConstant, m_globalCameraTransformConstantBuffer);
		}
	}

	void D3D11Graphics::CreateGlobalSceneDataConstantBuffer(const GlobalSceneDataConstant& globalSceneDataConstant)
	{
		if (m_globalSceneDataConstantBuffer == nullptr)
		{
			CreateConstantBuffer(globalSceneDataConstant, m_globalSceneDataConstantBuffer);
		}
		else
		{
			m_globalSceneDataConstantBuffer.Get()->Release();
			CreateConstantBuffer(globalSceneDataConstant, m_globalSceneDataConstantBuffer);
		}
	}

	void D3D11Graphics::SetGlobalConstantBuffers()
	{
		// 전역 상수 버퍼 파이프라인에 바인딩.
		// Common.hlsli에 정의해 준 것처럼 항상 register(b2), register(b3)에 들어가도록 함.
		m_context->VSSetConstantBuffers(2, 1, m_globalCameraTransformConstantBuffer.GetAddressOf());
		m_context->PSSetConstantBuffers(2, 1, m_globalCameraTransformConstantBuffer.GetAddressOf());
		m_context->GSSetConstantBuffers(2, 1, m_globalCameraTransformConstantBuffer.GetAddressOf());

		m_context->VSSetConstantBuffers(3, 1, m_globalSceneDataConstantBuffer.GetAddressOf());
		m_context->PSSetConstantBuffers(3, 1, m_globalSceneDataConstantBuffer.GetAddressOf());
		m_context->GSSetConstantBuffers(3, 1, m_globalSceneDataConstantBuffer.GetAddressOf());
	}

	void D3D11Graphics::UpdateGlobalCameraTransformConstantBuffer(const GlobalCameraTransformConstant& globalCameraTransformConstant)
	{
		UpdateBuffer(globalCameraTransformConstant, m_globalCameraTransformConstantBuffer);
	}

	void D3D11Graphics::UpdateGlobalSceneDataConstantBuffer(const GlobalSceneDataConstant& globalSceneDataConstant)
	{
		UpdateBuffer(globalSceneDataConstant, m_globalSceneDataConstantBuffer);
	}

	float D3D11Graphics::GetAspectRatio()
	{
		return float(m_screenWidth) / m_screenHeight;
	}

	void D3D11Graphics::ResizeScreen(int screenWidth, int screenHeight)
	{
		if (m_swapChain == nullptr) return;

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_backBufferRTV.Reset();
		m_swapChain->ResizeBuffers(0, // 현재 개수 유지
			m_screenWidth, // 해상도 변경
			m_screenHeight,
			DXGI_FORMAT_UNKNOWN, // 현재 포맷 유지
			0);

		CreateRenderTargetView();
		CreateDepthBuffer(m_screenWidth, m_screenHeight);
		SetViewport(m_screenWidth, m_screenHeight);
	}

#pragma region Pipeline Functions

	// 쉐이더 컴파일 에러 체크 함수.
	void CheckResult(HRESULT hr, ID3DBlob* errorBlob) 
	{
		if (FAILED(hr)) 
		{
			// 쉐이더 파일이 없을 경우
			if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) 
			{
				cout << "Shader Compile Error! : Shader file not found.\n";
			}

			// 에러 메시지가 있으면 출력
			if (errorBlob) 
			{
				cout << "Shader Compile Error! : Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << '\n';
			}
		}
	}

	void D3D11Graphics::CreateVertexShaderAndInputLayout(
		const wstring& filename,
		const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& vertexShader,
		ComPtr<ID3D11InputLayout>& inputLayout)
	{
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		// 쉐이더의 시작점의 이름이 "main"인 함수로 지정
		// D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용 가능하도록 하여 Common.hlsli 포함.
		// 쉐이더에서 헤더 파일 사용 시 플래그를 아래와 같이 지정해주지 않으면 shader file not found 에러가 발생 했음.
		HRESULT hr = D3DCompileFromFile(
			filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
			"vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob.Get());

		m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);

		m_device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			&inputLayout);
	}

	void D3D11Graphics::CreatePixelShader(const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
	{
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = D3DCompileFromFile(
			filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
			"ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob.Get());

		m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
	}

	void D3D11Graphics::CreateIndexBuffer(const vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& m_indexBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 인덱스 버퍼 또한 정점 버퍼와 같이 한 번 GPU로 보내면 바꿀 필요가 없음.
		bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; 
		bufferDesc.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		m_device->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
	}

	bool D3D11Graphics::CreateRenderTargetView()
	{
		// 스왑 체인에 들어 있는 Backbuffer를 얻어와 이를 렌더 타겟으로 설정함.
		ID3D11Texture2D* pBackBuffer;
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (pBackBuffer)
		{
			m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backBufferRTV);
			pBackBuffer->Release(); // 임시로 Backbuffer를 얻어와 저장한 텍스쳐 자원 해제.
		}
		else
		{
			cout << "CreateRenderTargetView() failed.\n";
			__ERRORLINE__
				return false;
		}

		return true;
	}

	void D3D11Graphics::SetViewport(int screenWidth, int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
		m_screenViewport.TopLeftX = 0;
		m_screenViewport.TopLeftY = 0;
		m_screenViewport.Width = static_cast<float>(m_screenWidth);
		m_screenViewport.Height = static_cast<float>(m_screenHeight);
		m_screenViewport.MinDepth = 0.0f;
		m_screenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

		m_context->RSSetViewports(1, &m_screenViewport);
	}

	bool D3D11Graphics::CreateDepthBuffer(int screenWidth, int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = m_screenWidth;
		depthStencilBufferDesc.Height = m_screenHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (m_numQualityLevels > 0)  // MSAA 사용 여부에 따라 다르게 설정.
		{
			depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
			depthStencilBufferDesc.SampleDesc.Quality = m_numQualityLevels - 1;
		}
		else
		{
			depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
			depthStencilBufferDesc.SampleDesc.Quality = 0;
		}
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		// 깊이 값을 저장할 버퍼 용도의 텍스쳐 자원을 생성.
		THROWFAILED(m_device->CreateTexture2D(&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf()));
		// 버퍼 용도로 만들어준 텍스쳐 자원을 DepthStencil 값을 쓸 목적임을 알려주기 위해 해당 버퍼 텍스쳐를 DepthStencilView에 연결.
		THROWFAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView));

		return true;
	}

	void D3D11Graphics::CreateTexture(
		const std::string filename,
		ComPtr<ID3D11Texture2D>& texture,
		ComPtr<ID3D11ShaderResourceView>& textureResourceView) 
	{
		int width, height, channels;

		unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		//assert(channels == 4);

		// 4채널로 만들어서 복사
		std::vector<uint8_t> image;
		image.resize(width * height * 4);
		for (size_t i = 0; i < width * height; i++) 
		{
			for (size_t c = 0; c < 3; c++) 
			{
				image[4 * i + c] = img[i * channels + c];
			}
			image[4 * i + 3] = 255;
		}

		// Create texture.
		D3D11_TEXTURE2D_DESC txtDesc = {};
		txtDesc.Width = width;
		txtDesc.Height = height;
		txtDesc.MipLevels = txtDesc.ArraySize = 1;
		txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		txtDesc.SampleDesc.Count = 1;
		txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
		txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = image.data();
		initData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
		// initData.SysMemSlicePitch = 0;

		m_device->CreateTexture2D(&txtDesc, &initData, texture.GetAddressOf());
		m_device->CreateShaderResourceView(texture.Get(), nullptr,
			textureResourceView.GetAddressOf());
	}

#pragma endregion Pipeline Functions
}