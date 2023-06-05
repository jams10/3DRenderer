#pragma once

#include "Utility/CustomMacros.h"

#include <queue>
#include <bitset>
#include <optional>

/*
 *	<Keyboard>
 *	Ű���� �Է��� ó���ϴ� Ŭ����.
 */
namespace NS
{
	class Keyboard
	{
		friend class Window;	// Window Ŭ������ private ����� ������ �� �ֵ��� ����.
	public:
#pragma region KeyEvent Class
		class KeyEvent          // Ű �Է½� �߻��ϴ� �̺�Ʈ�� ��Ÿ���� Ŭ����.
		{
		public:
			enum class Type			// Ű �̺�Ʈ�� Ÿ��.
			{
				Press,				// Ű ����.
				Release,			// Ű ��.
				Invalid				// �ʱ�ȭ ��. �ƹ� �͵� �ƴ� ����.
			};
			Type type;
			unsigned char code;		// Ű Event�� �߻���Ų Ű �ڵ带 ��� �ֵ��� ��.

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
		void Flush(); // key, char�� �Ѵ� flush
#pragma endregion
#pragma region AutoRepeat
		void EnableAutorepeat();
		void DisableAutorepeat();
		bool AutorepeatIsEnabled() const;
#pragma endregion
	private: // Window Ŭ������ ����� �Լ���.
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(char character);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);
	private:
		static constexpr unsigned int nKeys = 256u;       // ���� Ű �ڵ��� ����. 1����Ʈ 256��.
		static constexpr unsigned int bufferSize = 16u;   // Ű �̺�Ʈ�� �������� ������ �ִ� ũ��.
		bool autorepeatEnabled = false;                   // autorepeatEnabled üũ ����.
		std::bitset<nKeys> keystates;                     // �ش� ���� Ű�� ���ȴ��� ���θ� ������ bitset.
		std::queue<KeyEvent> keybuffer;                   // Ű Event�� ������ �� ����.
		std::queue<char> charbuffer;                      // Ű Event�� ���� char�� �������� ����.
	};
}



