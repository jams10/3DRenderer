#include "D3D11Graphics.h"

#include <d3dcompiler.h>
#include <iostream>
#include <string>

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
	{}

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
		sd.BufferDesc.Width = screenWidth;                 // set the back buffer width
		sd.BufferDesc.Height = screenHeight;               // set the back buffer height
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

		SetViewport(screenWidth, screenHeight);

#pragma region Depth Buffer
		CreateDepthBuffer(screenWidth, screenHeight);

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

		return true;
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
		if (m_screenWidth != screenWidth) m_screenWidth = screenWidth;
		if (m_screenHeight != screenHeight) m_screenHeight = screenHeight;

		ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
		m_screenViewport.TopLeftX = 0;
		m_screenViewport.TopLeftY = 0;
		m_screenViewport.Width = float(screenWidth);
		m_screenViewport.Height = float(screenHeight);
		m_screenViewport.MinDepth = 0.0f;
		m_screenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

		m_pContext->RSSetViewports(1, &m_screenViewport);
	}

	bool D3D11Graphics::CreateDepthBuffer(int screenWidth, int screenHeight)
	{
		if (m_screenWidth != screenWidth) m_screenWidth = screenWidth;
		if (m_screenHeight != screenHeight) m_screenHeight = screenHeight;

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = screenWidth;
		depthStencilBufferDesc.Height = screenHeight;
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

	void D3D11Graphics::BeginFrame(float red, float green, float blue, float alpha)
	{
		//RS: Rasterizer stage
		// OM: Output-Merger stage
		// VS: Vertex Shader
		// PS: Pixel Shader
		// IA: Input-Assembler stage

		SetViewport(m_screenHeight, m_screenHeight); // 그려줄 영역인 뷰포트 설정.

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

	void D3D11Graphics::EndFrame()
	{
		// Backbuffer에 그려진 화면을 모니터에 출력.
		m_pSwapChain->Present(0u, 0u);
	}
}