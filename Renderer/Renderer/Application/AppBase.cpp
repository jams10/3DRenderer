#include "AppBase.h"

#include <iostream>

#include "Graphics/Graphics.h"
#include "Graphics/D3D11Graphics.h"

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
		SAFE_RELEASE(m_pGraphics);
	}

	bool AppBase::Initialize()
	{
		// ������ �ʱ�ȭ.
		if (window.Initialize(L"Renderer", m_screenWidth, m_screenHeight) == false)
		{
			cout << "Failed to Initialize Window\n";
			__ERRORLINE__
			return false;
		}

		// �׷��Ƚ� �ʱ�ȭ.
		m_pGraphics = new Graphics();
		if (m_pGraphics->Initialize(m_screenWidth, m_screenHeight, window.GetWindowHandle()) == false)
		{
			cout << "Failed to Initialize Graphics\n";
			__ERRORLINE__
			return false;
		}

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

			Update(m_gameTimer.GetDeltaTime());

			m_pGraphics->GetD3D11()->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			Render();

			m_pGraphics->GetD3D11()->EndFrame();
		}
	}
}