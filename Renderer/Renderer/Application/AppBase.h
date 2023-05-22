#pragma once

#include "Utility/CustomMacros.h"
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
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

	protected:
		Window window;
		Graphics* m_pGraphics;
		int m_screenWidth;
		int m_screenHeight;
	};
}


