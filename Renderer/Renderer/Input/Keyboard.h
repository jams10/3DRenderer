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
		bool m_keyPressed[256] = {
			false,
		};
		// ������ �� Ű�� üũ��. ��� �۾��� ���� ���.
		bool m_keyToggle[256] = {
			false,
		};
	};
}



