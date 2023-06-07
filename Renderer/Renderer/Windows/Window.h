#pragma once

#include "WindowsHeaders.h"
#include "Utility/CustomMacros.h"

#include <string>

/*
*   ������ ����, �޽��� ó���� ����ϴ� Ŭ����.
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
		// ������ Ŭ������ ���/������ �����ϴ� �̱���.
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
		static int ProcessMessages() ; // ��� �����쿡 ���� �޽����� ó���ؾ� �ϹǷ� static���� ������.
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


