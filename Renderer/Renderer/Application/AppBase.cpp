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
		// Imgui�� ���� �� renderer backend(DX11)�� platform backend(win32) ���� ���� �ݾ��־�� ��.
		if (m_pGraphics != nullptr)
			m_pGraphics->Shutdown();

		window.ShutDownImGui();

		SAFE_RELEASE(m_pGraphics);
	}

	bool AppBase::Initialize()
	{
		// ������ �ʱ�ȭ.
		if (window.Initialize(L"Renderer", m_screenWidth, m_screenHeight, &m_keyboard) == false)
		{
			cout << "Failed to Initialize Window\n";
			__ERRORLINE__
			return false;
		}

		// �׷��Ƚ� �ʱ�ȭ.
		m_pGraphics = new GraphicsProcessor();
		if (m_pGraphics->Initialize(m_screenWidth, m_screenHeight, window.GetWindowHandle()) == false)
		{
			cout << "Failed to Initialize Graphics\n";
			__ERRORLINE__
			return false;
		}

		// PSO ���� �� �ʱ�ȭ.
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
			if (window.ProcessMessages() == 0) // ProcessMessages���� WM_QUIT �޽����� ������ 0�� ������.
				return 0;

			ImGui_ImplDX11_NewFrame(); // GUI ������ ����
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame(); // IMGUI ������ ����.
			UpdateGUI(); // GUI �߰�.
			ImGui::Render(); // �������� �͵� ��� ��.

			Update(m_gameTimer.GetDeltaTime()); // �� ������Ʈ.

			m_pGraphics->GetD3D11()->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			Render(); // �� ������.

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI ������.

			m_pGraphics->GetD3D11()->EndFrame();
		}
	}
}