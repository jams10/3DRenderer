#include "AppBase.h"

#include <iostream>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"

namespace NS
{
	using std::cout;

	AppBase::AppBase(int screenWidth, int screenHeight)
		:
		m_screenWidth(screenWidth),
		m_screenHeight(screenHeight),
		window(),
		m_pGraphics(nullptr)
	{}

	AppBase::~AppBase()
	{
		// Imgui를 닫을 때 renderer backend(DX11)를 platform backend(win32) 보다 먼저 닫아주어야 함.
		if (m_pGraphics != nullptr)
			m_pGraphics->Shutdown();

		window.ShutDownImGui();

		SAFE_RELEASE(m_pGraphics);
	}

	bool AppBase::Initialize()
	{
		// 윈도우 초기화.
		if (window.Initialize(L"Renderer", m_screenWidth, m_screenHeight, &m_keyboard) == false)
		{
			cout << "Failed to Initialize Window\n";
			__ERRORLINE__
			return false;
		}

		// 그래픽스 초기화.
		m_pGraphics = new GraphicsProcessor();
		if (m_pGraphics->Initialize(m_screenWidth, m_screenHeight, window.GetWindowHandle()) == false)
		{
			cout << "Failed to Initialize Graphics\n";
			__ERRORLINE__
			return false;
		}

		// PSO 생성 및 초기화.
		Graphics::InitCommonStates(m_pGraphics);

		return true;
	}

	void AppBase::Update(float dt)
	{
		if (window.bShouldResizeScreen)
		{
			m_pGraphics->GetD3D11()->ResizeScreen(m_screenWidth, m_screenHeight);
			window.bShouldResizeScreen = false;
		}
	}

	int AppBase::Run()
	{
		m_gameTimer.Tick();

		while (true)
		{
			if (window.ProcessMessages() == 0) // ProcessMessages에서 WM_QUIT 메시지를 받으면 0을 리턴함.
				return 0;

			ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame(); // IMGUI 렌더링 시작.
			UpdateGUI(); // GUI 추가.
			ImGui::Render(); // 렌더링할 것들 기록 끝.

			Update(m_gameTimer.GetDeltaTime()); // 씬 업데이트.

			m_pGraphics->GetD3D11()->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			Render(); // 씬 렌더링.

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링.

			m_pGraphics->GetD3D11()->EndFrame();
		}
	}
}