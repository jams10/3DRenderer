#include "AppBase.h"

#include <iostream>

#include "Graphics/Graphics.h"

namespace NS
{
	using std::cout;

	AppBase::AppBase(int screenWidth, int screenHeight)
		:
		m_screenWidth(screenWidth),
		m_screenHeight(screenHeight),
		window()
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

	int AppBase::Run()
	{
		while (true)
		{
			if (window.ProcessMessages() == 0) // ProcessMessages���� WM_QUIT �޽����� ������ 0�� ������.
				return 0;

			m_pGraphics->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

			m_pGraphics->EndFrame();
		}
	}
}