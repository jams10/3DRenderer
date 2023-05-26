#include "D3D11Graphics.h"

#include <d3dcompiler.h>
#include <iostream>
#include <string>
#include <imgui_impl_dx11.h>

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
		THROWFAILED(device.As(&m_pDevice));

		THROWFAILED(context.As(&m_pContext));
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
					&sd, &m_pSwapChain, &m_pDevice, &featureLevel,
					&m_pContext));
#pragma endregion Create Swapchain & Backbuffer

		if (CreateRenderTargetView() == false) return false;

		SetViewport(m_screenWidth, m_screenHeight);

#pragma region Depth Buffer
		CreateDepthBuffer(m_screenWidth, m_screenHeight);

		// Depth Stencil State(깊이 값을 어떻게 비교할 것인가를 결정)를 생성.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = true; // false
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		THROWFAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, m_pDepthStencilState.GetAddressOf()));
#pragma endregion Depth Buffer

		// Rastersizer State(어떤 방식으로 래스터화 할 것인가를 결정) 생성.
		D3D11_RASTERIZER_DESC rastDesc;
		ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // 서술자 초기화.
		rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rastDesc.FrontCounterClockwise = false;

		m_pDevice->CreateRasterizerState(&rastDesc, &m_pSolidRasterizerSate);

		// imgui dx11 구현 초기화.
		if (ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get()) == false)
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
		//RS: Rasterizer stage
		// OM: Output-Merger stage
		// VS: Vertex Shader
		// PS: Pixel Shader
		// IA: Input-Assembler stage

		SetViewport(m_screenWidth, m_screenHeight); // 그려줄 영역인 뷰포트 설정.

		// RTV와 DSV 초기화. 화면을 지정 색상으로 날려주고, 깊이 버퍼도 초기화 해줌.
		float clearColor[4] = { red, green, blue, alpha };
		m_pContext->ClearRenderTargetView(m_pBackBufferRTV.Get(), clearColor);
		m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// 씬을 렌더링 할 렌더 타겟 설정, 깊이 값 기록시 어떻게 기록할 지 DepthStencil State도 설정.
		ID3D11RenderTargetView* targets[] = { m_pBackBufferRTV.Get() };
		m_pContext->OMSetRenderTargets(1, targets, m_pDepthStencilView.Get());
		m_pContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

		// 어떻게 래스터화 할 지 Rasterizer State도 설정.
		m_pContext->RSSetState(m_pSolidRasterizerSate.Get());
	}

	void D3D11Graphics::Render(const MeshForCPU& meshForCPU, const MeshForGPU& meshForGPU)
	{
		// 정점 쉐이더 파이프라인에 바인딩.
		m_pContext->VSSetShader(meshForGPU.pVertexShader.Get(), 0, 0);
		// 상수 버퍼 파이프라인에 바인딩.
		m_pContext->VSSetConstantBuffers(0, 1, meshForGPU.pConstantBuffer.GetAddressOf());
		// 픽셀 쉐이더 파이프라인에 바인딩.
		m_pContext->PSSetShader(meshForGPU.pPixelShader.Get(), 0, 0);

		// 정점, 인덱스 버퍼 설정하고 그리기 명령 호출.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_pContext->IASetInputLayout(meshForGPU.pInputLayout.Get());
		m_pContext->IASetVertexBuffers(0, 1, meshForGPU.pVertexBuffer.GetAddressOf(), &stride, &offset);
		m_pContext->IASetIndexBuffer(meshForGPU.pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->DrawIndexed(meshForCPU.indexCount, 0, 0);
	}

	void D3D11Graphics::EndFrame()
	{
		// Backbuffer에 그려진 화면을 모니터에 출력.
		m_pSwapChain->Present(0u, 0u);
	}

	float D3D11Graphics::GetAspectRatio()
	{
		return float(m_screenWidth) / m_screenHeight;
	}

	void D3D11Graphics::ResizeScreen(float screenWidth, float screenHeight)
	{
		if (m_pSwapChain == nullptr) return;

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_pBackBufferRTV.Reset();
		m_pSwapChain->ResizeBuffers(0, // 현재 개수 유지
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
				cout << "File not found.\n";
			}

			// 에러 메시지가 있으면 출력
			if (errorBlob) 
			{
				cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << '\n';
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

		// 쉐이더 시작 점을 이름이 main 함수로 지정.
		HRESULT hr = D3DCompileFromFile(filename.c_str(), 0, 0, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob.Get());

		m_pDevice->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);

		m_pDevice->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			&inputLayout);
	}

	void D3D11Graphics::CreatePixelShader(const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
	{
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		HRESULT hr = D3DCompileFromFile(filename.c_str(), 0, 0, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

		CheckResult(hr, errorBlob.Get());

		m_pDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &pixelShader);
	}

	void D3D11Graphics::CreateIndexBuffer(const vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& m_indexBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 인덱스 버퍼 또한 정점 버퍼와 같이 한 번 GPU로 보내면 바꿀 필요가 없음.
		bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; 
		bufferDesc.StructureByteStride = sizeof(uint16_t);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		m_pDevice->CreateBuffer(&bufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
	}

	bool D3D11Graphics::CreateRenderTargetView()
	{
		// 스왑 체인에 들어 있는 Backbuffer를 얻어와 이를 렌더 타겟으로 설정함.
		ID3D11Texture2D* pBackBuffer;
		m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (pBackBuffer)
		{
			m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBufferRTV);
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

		m_pContext->RSSetViewports(1, &m_screenViewport);
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
		THROWFAILED(m_pDevice->CreateTexture2D(&depthStencilBufferDesc, 0, m_pDepthStencilBuffer.GetAddressOf()));
		// 버퍼 용도로 만들어준 텍스쳐 자원을 DepthStencil 값을 쓸 목적임을 알려주기 위해 해당 버퍼 텍스쳐를 DepthStencilView에 연결.
		THROWFAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), 0, &m_pDepthStencilView));

		return true;
	}

#pragma endregion Pipeline Functions
}