#pragma once

#include "Utility/CustomMacros.h"
#include "Utility/GameTimer.h"
#include "Windows/Window.h"

namespace NS
{
	class Graphics;

	class AppBase
	{
	public:
		AppBase(int screenWidth, int screenHeight);
		AppBase(const AppBase&) = delete;
		AppBase& operator=(const AppBase&) = delete;
		virtual ~AppBase();

		int Run();

		virtual bool Initialize();
		virtual void Update(float dt);
		virtual void Render() = 0;

	protected:
		Window window;
		Graphics* m_pGraphics;
		GameTimer m_gameTimer;
		int m_screenWidth;
		int m_screenHeight;
	};
}


