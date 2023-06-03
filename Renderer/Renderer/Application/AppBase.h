#pragma once

#include "Utility/CustomMacros.h"
#include "Utility/GameTimer.h"
#include "Windows/Window.h"
#include "UI/ImGuiManager.h"

/*
*	<AppBase>
*	기본 Application 클래스.
*	윈도우 창 생성을 위한 Window, 렌더링을 위한 Graphics, ImGui 초기화를 위한 ImGui 매니저 인스턴스를 생성, 초기화를 담당함.
*/

namespace NS
{
	class GraphicsProcessor;

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
		GraphicsProcessor* m_pGraphics;
		ImGuiManager m_imGuiManager;
		GameTimer m_gameTimer;
		int m_screenWidth;
		int m_screenHeight;
	};
}


