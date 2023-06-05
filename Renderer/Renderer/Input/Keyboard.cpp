#include "Keyboard.h"

namespace NS
{
	// ���� Ű �ڵ带 �Է� �޾� �ش� Ű�� ���ȴ��� ���θ� �������ִ� �Լ�.
	bool Keyboard::KeyIsPressed(unsigned char keycode) const 
	{
		return keystates[keycode];
	}

	// Ű Event ���ۿ� ����� ���� ������ Event�� ���� �������ִ� �Լ�.
	std::optional<Keyboard::KeyEvent> Keyboard::ReadKey()
	{
		if (keybuffer.size() > 0u)
		{
			Keyboard::KeyEvent e = keybuffer.front();
			keybuffer.pop();
			return e;
		}
		return {};
	}

	// Ű Event ���۰� ������� ���θ� �������ִ� �Լ�.
	bool Keyboard::KeyIsEmpty() const 
	{
		return keybuffer.empty();
	}

	// Ű char ���ۿ� �ִ� ���� ������ char ���� ���� �������ִ� �Լ�.
	char Keyboard::ReadChar() 
	{
		if (charbuffer.size() > 0u)
		{
			unsigned char charcode = charbuffer.front();
			charbuffer.pop();
			return charcode;
		}
		else
		{
			return 0;
		}
	}

	// Ű char ���۰� ������� ���θ� �������ִ� �Լ�.
	bool Keyboard::CharIsEmpty() const 
	{
		return charbuffer.empty();
	}

	// Ű Event ���۸� ����ִ� �Լ�.
	void Keyboard::FlushKey() 
	{
		keybuffer = std::queue<KeyEvent>();
	}

	// Ű char ���۸� ����ִ� �Լ�.
	void Keyboard::FlushChar() 
	{
		charbuffer = std::queue<char>();
	}

	// Ű Event, char ���۸� ��� ����ִ� �Լ�.
	void Keyboard::Flush() 
	{
		FlushKey();
		FlushChar();
	}

	// autorepeatEnabled�� ���ִ� �Լ�.
	void Keyboard::EnableAutorepeat() 
	{
		autorepeatEnabled = true;
	}

	// autorepeatEnabled�� ���ִ� �Լ�.
	void Keyboard::DisableAutorepeat() 
	{
		autorepeatEnabled = false;
	}

	// autorepeatEnabled ���¸� �������ִ� �Լ�.
	bool Keyboard::AutorepeatIsEnabled() const 
	{
		return autorepeatEnabled;
	}

	// �Է��� ���� Ű�� ���� Ű �ڵ带 �ϳ� �޾� Ű Event ���ۿ� �־��ִ� �Լ�.
	void Keyboard::OnKeyPressed(unsigned char keycode) 
	{
		keystates[keycode] = true;                                                     // �ش� ���� Ű �ڵ带 �ε����� �ϴ� ��ġ�� ���� true�� �ٲ���.
		keybuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::Type::Press, keycode));  // Event Ÿ���� Press, �ش� ���� Ű �ڵ�� �Բ� Event ���ۿ� �־���.
		TrimBuffer(keybuffer);                                                         // ���� ������ ������ ���� ũ�⸦ ����� �ʵ��� ����.
	}

	// �Է��� �� ���� Ű �ڵ带 �ϳ� �޾� Ű Event ���ۿ� �־��ִ� �Լ�.
	void Keyboard::OnKeyReleased(unsigned char keycode) 
	{
		keystates[keycode] = false;                                                     // �ش� ���� Ű �ڵ带 �ε����� �ϴ� ��ġ�� ���� false�� �ٲ���.
		keybuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::Type::Release, keycode)); // Event Ÿ���� Release, �ش� ���� Ű �ڵ�� �Բ� Event ���ۿ� �־���.
		TrimBuffer(keybuffer);                                                          // ���� ������ ������ ���� ũ�⸦ ����� �ʵ��� ����.
	}

	// �Է��� Ű�� char ���� �ϳ� �޾� char ���ۿ� �־��ִ� �Լ�.
	void Keyboard::OnChar(char character) 
	{
		charbuffer.push(character);                                                // Ű char ���ۿ� char ���� �־���.
		TrimBuffer(charbuffer);                                                    // ���� ������ ������ ���� ũ�⸦ ����� �ʵ��� ����.
	}

	// ��� ���� Ű ���¸� �������ִ� �Լ�. ��� ���� Ű ���°� false�� ��.
	void Keyboard::ClearState() 
	{
		keystates.reset();
	}

	template<typename T>
	void Keyboard::TrimBuffer(std::queue<T>& buffer) 
	{
		// �츮�� ������ ���� ũ�Ⱑ �� �� ���� ���ۿ� ����ִ� ���� ���� ������ �������� ������.
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}