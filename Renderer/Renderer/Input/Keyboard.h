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
		bool m_keyPressed[256] = {
			false,
		};
	};
}



