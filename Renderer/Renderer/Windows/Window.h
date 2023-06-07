#pragma once

#include "WindowsHeaders.h"
#include "Utility/CustomMacros.h"

#include <string>

/*
*   윈도우 생성, 메시지 처리를 담당하는 클래스.
*/
namespace NS
{
	using std::wstring;
	class Keyboard;
	class Mouse;

	class Window
	{
#pragma region WindowClass
	private:
		// 윈도우 클래스를 등록/해제를 관리하는 싱글톤.
		class WindowClass
		{
		public:
			static const wchar_t* GetName() ;
			static HINSTANCE GetInstance() ;
		private:
			WindowClass() ;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;

			static constexpr const wchar_t* wndClassName = L"WindowClass";
			static WindowClass wndClass; // SINGLETON
			HINSTANCE hInst;
		};
#pragma endregion

	public:
		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		bool Initialize(const wchar_t* name, int width, int height, Keyboard* pKeyboard, Mouse* pMouse);
		bool SetTitle(const wstring& title);
		static int ProcessMessages() ; // 모든 윈도우에 대한 메시지를 처리해야 하므로 static으로 선언함.
		HWND GetWindowHandle() { return m_hWnd; }
		void ShutDownImGui();

		bool bShouldResizeScreen = false;
		int m_screenWidth;
		int m_screenHeight;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) ;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) ;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) ;

	private:
		HWND m_hWnd;
		Keyboard* m_pKeyboard;
		Mouse* m_pMouse;
	};
}


