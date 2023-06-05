#pragma once

#include "Utility/CustomMacros.h"

#include <queue>
#include <bitset>
#include <optional>

/*
 *	<Keyboard>
 *	키보드 입력을 처리하는 클래스.
 */
namespace NS
{
	class Keyboard
	{
		friend class Window;	// Window 클래스가 private 멤버에 접근할 수 있도록 해줌.
	public:
#pragma region KeyEvent Class
		class KeyEvent          // 키 입력시 발생하는 이벤트를 나타내는 클래스.
		{
		public:
			enum class Type			// 키 이벤트의 타입.
			{
				Press,				// 키 누름.
				Release,			// 키 뗌.
				Invalid				// 초기화 값. 아무 것도 아닌 상태.
			};
			Type type;
			unsigned char code;		// 키 Event를 발생시킨 키 코드를 들고 있도록 함.

			KeyEvent()
				:
				type(Type::Invalid),
				code(0u)
			{}
			KeyEvent(Type type, unsigned char code)
				:
				type(type),
				code(code)
			{}
			bool IsPress() const
			{
				return type == Type::Press;
			}
			bool IsRelease() const
			{
				return type == Type::Release;
			}
			bool IsValid() const
			{
				return type != Type::Invalid;
			}
			unsigned char GetCode() const
			{
				return code;
			}
		};
#pragma endregion
	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
#pragma region KeyEvent
		bool KeyIsPressed(unsigned char keycode) const;
		std::optional<KeyEvent> ReadKey();
		bool KeyIsEmpty() const;
		void FlushKey() ;
#pragma endregion
#pragma region CharEvent
		char ReadChar() ;
		bool CharIsEmpty() const;
		void FlushChar();
		void Flush(); // key, char를 둘다 flush
#pragma endregion
#pragma region AutoRepeat
		void EnableAutorepeat();
		void DisableAutorepeat();
		bool AutorepeatIsEnabled() const;
#pragma endregion
	private: // Window 클래스가 사용할 함수들.
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(char character);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);
	private:
		static constexpr unsigned int nKeys = 256u;       // 가상 키 코드의 개수. 1바이트 256개.
		static constexpr unsigned int bufferSize = 16u;   // 키 이벤트를 저장해줄 버퍼의 최대 크기.
		bool autorepeatEnabled = false;                   // autorepeatEnabled 체크 변수.
		std::bitset<nKeys> keystates;                     // 해당 가상 키가 눌렸는지 여부를 저장할 bitset.
		std::queue<KeyEvent> keybuffer;                   // 키 Event를 저장해 줄 버퍼.
		std::queue<char> charbuffer;                      // 키 Event에 대한 char를 저장해줄 버퍼.
	};
}



