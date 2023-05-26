#pragma once

#include "Utility/CustomMacros.h"
#include "Utility/GameTimer.h"
#include "Windows/Window.h"
#include "UI/ImGuiManager.h"

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
		virtual void UpdateGUI() = 0;
		virtual void Update(float dt);
		virtual void Render() = 0;

	protected:
		Window window;
		Graphics* m_pGraphics;
		ImGuiManager m_imGuiManager;
		GameTimer m_gameTimer;
		int m_screenWidth;
		int m_screenHeight;
	};
}


