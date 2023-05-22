#include "AppBase.h"

#include <iostream>

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
	}

	bool AppBase::Initialize()
	{
		// 윈도우 초기화.
		if (window.Initialize(L"Renderer", m_screenWidth, m_screenHeight) == false)
		{
			cout << "Failed to Initialize Window\n";
			__ERRORLINE__
			return false;
		}
		return true;
	}

	int AppBase::Run()
	{
		while (true)
		{
			if (window.ProcessMessages() == 0) // ProcessMessages에서 WM_QUIT 메시지를 받으면 0을 리턴함.
				return 0;
		}
	}
}